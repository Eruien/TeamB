#include "SelectScene.h"
#include "LGlobal.h"
#include "LFbxMgr.h"
#include "LExportIO.h"
#include "LInput.h"

bool SelectScene::Init()
{
    InitializeDebugCamera();
    InitializeMap();
    InitializeModel();
    InitializeWeapon();
    InitializeLighting();
    return false;
}

void SelectScene::Process()
{
    m_CustomMap->Frame();
    m_GunMan->Frame();
    UpdateWeaponPosition();

    m_Select.SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
    m_GunMan->m_OBBBox.UpdateOBBBoxPosition(
        { m_GunMan->m_OBBBox.m_matWorld._41,
            m_GunMan->m_OBBBox.m_matWorld._42,
            m_GunMan->m_OBBBox.m_matWorld._43 });
   
    if (LInput::GetInstance().m_MouseState[0])
    {
        if (m_Select.ChkOBBToRay(&m_GunMan->m_OBBBox.m_Box))
        {
            return;
        }
    }
}

void SelectScene::Render()
{
    m_CustomMap->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
    m_CustomMap->Render();
    m_GunMan->Render();

    TMatrix playerTranslation;
    playerTranslation.Translation(TVector3(m_GunMan->m_matControl._41, m_GunMan->m_matControl._42 + m_GunMan->m_SettingBox.vCenter.y, m_GunMan->m_matControl._43));
    m_GunMan->m_OBBBox.SetMatrix(&playerTranslation, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
    m_GunMan->m_OBBBox.Render();
    m_Rifle->m_WeaponModel->Render();

    fLightStart += LGlobal::g_fSPF;
    if (fLightStart > fLightEnd)
    {
        m_PointLight[0].m_vPosition.y = -1000.f;
    }

    m_cbLight1.g_cAmbientMaterial[0] = TVector4(0.1f, 0.1f, 0.1f, 1);
    m_cbLight1.g_cDiffuseMaterial[0] = TVector4(0.6f);
    m_cbLight1.g_cSpecularMaterial[0] = TVector4(1, 1, 1, 1);
    m_cbLight1.g_cEmissionMaterial[0] = TVector4(0, 0, 0, 1);

    m_cbLight1.g_cAmbientLightColor[0] = TVector4(1, 1, 1, 1);
    m_cbLight1.g_cSpecularLightColor[0] = TVector4(1, 1, 1, 1);
    m_cbLight1.g_cDiffuseLightColor[0] = TVector4(m_PointLight[0].m_DiffuseColor.x,
        m_PointLight[0].m_DiffuseColor.y,
        m_PointLight[0].m_DiffuseColor.z, 1.0f);

    m_cbLight2.g_vLightPos[0] = TVector4(m_PointLight[0].m_vPosition.x,
        m_PointLight[0].m_vPosition.y,
        m_PointLight[0].m_vPosition.z,
        m_PointLight[0].m_fRange);

    m_cbLight2.g_vLightDir[0] = TVector4(m_PointLight[0].m_vDirection.x,
        m_PointLight[0].m_vDirection.y,
        m_PointLight[0].m_vDirection.z, 1.0f);
   
    LGlobal::g_pImmediateContext->UpdateSubresource(m_pConstantBufferLight[0].Get(), 0, NULL, &m_cbLight1, 0, 0);
    LGlobal::g_pImmediateContext->UpdateSubresource(m_pConstantBufferLight[1].Get(), 0, NULL, &m_cbLight2, 0, 0);

    ID3D11Buffer* pBuffers[2];
    pBuffers[0] = m_pConstantBufferLight[0].Get();
    pBuffers[1] = m_pConstantBufferLight[1].Get();
    LGlobal::g_pImmediateContext->PSSetConstantBuffers(3, 2, pBuffers);
}

void SelectScene::Release()
{
}

void SelectScene::InitializeDebugCamera()
{
    m_DebugCamera = std::make_shared<LDebugCamera>();
    m_DebugCamera->CreateLookAt({ 0.0f, 200.0f, -100.0f }, { 0.0f, 0.0f, 1.0f });
    m_DebugCamera->CreatePerspectiveFov(L_PI * 0.25, (float)LGlobal::g_WindowWidth / (float)LGlobal::g_WindowHeight, 1.0f, 10000.0f);
    m_DebugCamera->m_vCameraPos = { 15.186f, 100.445, 59.244 };
    m_DebugCamera->m_fCameraYaw = 3.70707107f;
    m_DebugCamera->m_fCameraPitch = 44.4638596f;
    m_DebugCamera->m_fCameraRoll = 0.0f;
    LGlobal::g_pMainCamera = m_DebugCamera.get();
}

void SelectScene::InitializeMap()
{
    m_CustomMap = std::make_shared<LHeightMap>();
    m_CustomMap->Set();
    m_CustomMap->CreateHeightMap(L"../../res/Heightmap/heightMap513.bmp");
    //map
    LMapDesc MapDesc = {};
    MapDesc.iNumCols = m_CustomMap->m_iNumCols;
    MapDesc.iNumRows = m_CustomMap->m_iNumRows;
    MapDesc.fCellDistance = 4.0f;
    MapDesc.fScaleHeight = 0.4f;
    MapDesc.ShaderFilePath = L"../../res/hlsl/CustomizeMap.hlsl";
    MapDesc.TextureFilePath = L"../../res/map/aerial_grass_rock_diff_8k.jpg";
    m_CustomMap->Load(MapDesc);
}

void SelectScene::InitializeModel()
{
    // PlayerSetting
    m_GunMan = std::make_shared<LSkinningModel>();
    m_GunMan->m_pModel = LFbxMgr::GetInstance().GetPtr(L"army3.fbx");
    m_GunMan->CreateBoneBuffer();
    m_GunMan->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Idle_Rifle_Ironsights.fbx");
    TMatrix matScale;
    TMatrix matRot;
    D3DXMatrixScaling(&matScale, 0.2f, 0.2f, 0.2f);
    m_GunMan->m_matControl *= matScale;
    D3DXMatrixRotationY(&matRot, 3.14159);
    m_GunMan->m_matControl *= matRot;
    m_GunMan->m_matControl._42 += 50.0f;

    std::wstring head = L"Head";
    std::wstring root = L"root";

    TMatrix Head = m_GunMan->m_pModel->m_NameMatrixMap[0][head];
    TMatrix Root = m_GunMan->m_pModel->m_NameMatrixMap[0][root];

    m_GunMan->SetOBBBox({ -30.0f, Root._42, -20.0f }, { 30.0f, Head._42, 30.0f }, 0.2f);

    m_GunMan->m_OBBBox.CreateOBBBox(
       m_GunMan->m_SettingBox.fExtent[0],
       m_GunMan->m_SettingBox.fExtent[1],
       m_GunMan->m_SettingBox.fExtent[2],
        {m_GunMan->m_OBBBox.m_matWorld._41,
           m_GunMan->m_OBBBox.m_matWorld._42,
           m_GunMan->m_OBBBox.m_matWorld._43 },
       m_GunMan->m_SettingBox.vAxis[0],
       m_GunMan->m_SettingBox.vAxis[1],
       m_GunMan->m_SettingBox.vAxis[2]);
}

void SelectScene::InitializeWeapon()
{
    m_Rifle = std::make_shared<LWeapon>();
    m_Rifle->m_WeaponModel = std::make_shared<LModel>();
    m_Rifle->m_WeaponModel->m_pModel = LFbxMgr::GetInstance().GetPtr(L"Assault_Rifle_A.fbx");

    m_Rifle->m_WeaponModel->m_ParentBoneName = LExportIO::GetInstance().m_ItemParentName[0];
    m_Rifle->m_WeaponModel->m_pModel->m_matScale = LExportIO::GetInstance().m_ItemScale[0];
    m_Rifle->m_WeaponModel->m_pModel->m_matRotation = LExportIO::GetInstance().m_ItemRotation[0];
    m_Rifle->m_WeaponModel->m_pModel->m_matTranslation = LExportIO::GetInstance().m_ItemTranslation[0];
}

void SelectScene::UpdateWeaponPosition()
{
    if (m_Rifle->m_WeaponModel->m_pModel != nullptr && m_GunMan->m_pActionModel != nullptr)
    {
        if (m_GunMan->m_pActionModel->m_iEndFrame <= int(m_GunMan->m_fCurrentAnimTime)) return;

        m_Rifle->m_WeaponModel->m_pModel->m_matSocket =
        m_GunMan->m_pActionModel->m_NameMatrixMap[int(m_GunMan->m_fCurrentAnimTime)][m_Rifle->m_WeaponModel->m_ParentBoneName];

        m_Rifle->m_WeaponModel->m_matControl = m_Rifle->m_WeaponModel->m_pModel->m_matScale * m_Rifle->m_WeaponModel->m_pModel->m_matRotation * m_Rifle->m_WeaponModel->m_pModel->m_matSocket
            * m_Rifle->m_WeaponModel->m_pModel->m_matTranslation * m_GunMan->m_matControl;
    }
}

void SelectScene::InitializeLighting()
{
    m_pConstantBufferLight[0].Attach(CreateConstantBuffer(
        LGlobal::g_pDevice.Get(), &m_cbLight1, 1, sizeof(LIGHT_CONSTANT_BUFFER1)));
    m_pConstantBufferLight[1].Attach(CreateConstantBuffer(
        LGlobal::g_pDevice.Get(), &m_cbLight2, 1, sizeof(LIGHT_CONSTANT_BUFFER2)));
    float fLightRange = 50.0f;
    TVector3 vRotation = TVector3(0.f, 0.0f, 0.0f); //TVector3(-(XM_PI * 0.2f), 0.0f, 0.0f);
    TVector3 vDir = TVector3(0.0f, -1.0f, 0.0f);
    TVector3 v0 = TVector3(0.0f, 10.0f, 0.0f);
    TVector4 v1 = TVector4(1.0f, 1.0f, 1.0f, 1.0f); // color
    TVector3 v2 = TVector3(10.0f, 10.0f, 10.0f);    // scale
    m_PointLight[0].SetValue(&v0,
        &vDir,
        &fLightRange,
        &v1,
        &v2,
        &vRotation,
        90.0f, // 내부
        120.0f);//외부
}

ID3D11Buffer* SelectScene::CreateConstantBuffer(ID3D11Device* pd3dDevice, void* data, UINT iNumIndex, UINT iSize, bool bDynamic)
{
    HRESULT hr = S_OK;
    ID3D11Buffer* pBuffer = nullptr;
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    if (bDynamic)
    {
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    }
    else
    {
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.CPUAccessFlags = 0;
    }
    bd.ByteWidth = iSize * iNumIndex;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    InitData.pSysMem = data;
    if (data != NULL)
    {
        if (FAILED(hr = pd3dDevice->CreateBuffer(&bd, &InitData, &pBuffer)))
        {
            //H(hr);
            return nullptr;
        }
    }
    else
    {
        if (FAILED(hr = pd3dDevice->CreateBuffer(&bd, NULL, &pBuffer)))
        {
            //H(hr);
            return nullptr;
        }
    }
    return pBuffer;
}

SelectScene::SelectScene(LScene* parent) : SceneState(parent)
{
    Init();
}

SelectScene::~SelectScene()
{

}
