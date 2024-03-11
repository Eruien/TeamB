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
    MapDesc.ShaderFilePath = L"../../res/hlsl/LightShadowMap.hlsl";
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

SelectScene::SelectScene(LScene* parent) : SceneState(parent)
{
    Init();
}

SelectScene::~SelectScene()
{

}
