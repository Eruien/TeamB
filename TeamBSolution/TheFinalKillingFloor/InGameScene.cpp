#include "InGameScene.h"
#include "LGlobal.h"
#include "LInput.h"
#include "LWrite.h"
#include "UIManager.h"
#include "LCharacterIO.h"
#include "LAnimationIO.h"
#include "LExportIO.h"

bool InGameScene::Init()
{
    m_DebugCamera = std::make_shared<LDebugCamera>();
    m_DebugCamera->CreateLookAt({ 0.0f, 200.0f, -100.0f }, { 0.0f, 0.0f, 1.0f });
    m_DebugCamera->CreatePerspectiveFov(L_PI * 0.25, (float)LGlobal::g_WindowWidth / (float)LGlobal::g_WindowHeight, 1.0f, 10000.0f);

    m_ModelCamera = std::make_shared<LModelCamera>();
    m_ModelCamera->SetTargetPos(TVector3(0.0f, 0.0f, 0.0f));
    m_ModelCamera->CreatePerspectiveFov(L_PI * 0.25, (float)LGlobal::g_WindowWidth / (float)LGlobal::g_WindowHeight, 1.0f, 10000.0f);
    m_ModelCamera->m_fRadius = 100.0f;
    LGlobal::g_pMainCamera = m_ModelCamera.get();

    CreateShadowConstantBuffer();

    m_SkyBox = std::make_shared<LSkyBox>();
    m_SkyBox->Set();
    m_SkyBox->Create(L"../../res/hlsl/SkyBox.hlsl", L"../../res/sky/grassenvmap1024.dds");

    LCharacterIO::GetInstance().CharacterRead(L"../../res/UserFile/Character/army3.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Fire_Rifle_Ironsights.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Reload_Rifle_Ironsights.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Sprint_Fwd_Rifle.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Walk_Fwd_Rifle_Ironsights.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Idle_Rifle_Ironsights.bin");
    LCharacterIO::GetInstance().ItemRead(L"../../res/UserFile/Item/Assault_Rifle_A.bin");

    LCharacterIO::GetInstance().CharacterRead(L"../../res/UserFile/Character/Zombie.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Zombie_Attack_Anim.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Zombie_Death.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Zombie_TakeDamage.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Zombie_Walk_Lock.bin");

    LExportIO::GetInstance().ExportRead(L"RightHandForm.bin");
   
    m_PlayerModel = std::make_shared<LPlayer>();
    m_PlayerModel->m_pModel = LFbxMgr::GetInstance().GetPtr(L"army3.fbx");
    m_PlayerModel->CreateBoneBuffer();
    m_PlayerModel->FSM(FSMType::PLAYER);

    TMatrix matScale;
    TMatrix matRot;
    D3DXMatrixScaling(&matScale, 0.2f, 0.2f, 0.2f);
    m_PlayerModel->m_matControl *= matScale;
    D3DXMatrixRotationY(&matRot, 3.14159);
    m_PlayerModel->m_matControl *= matRot;

    m_GunModel = std::make_shared<LModel>();
    m_GunModel->m_pModel = LFbxMgr::GetInstance().GetPtr(L"Assault_Rifle_A.fbx");

    m_GunModel->m_ParentBoneName = LExportIO::GetInstance().m_ItemParentName[0];
    m_GunModel->m_pModel->m_matScale = LExportIO::GetInstance().m_ItemScale[0];
    m_GunModel->m_pModel->m_matRotation = LExportIO::GetInstance().m_ItemRotation[0];
    m_GunModel->m_pModel->m_matTranslation = LExportIO::GetInstance().m_ItemTranslation[0];

   /* D3DXMatrixRotationY(&matRot, -3.14159);
    m_GunModel->m_pModel->m_matRotation *= matRot;*/

    m_ZombieModelList.resize(m_EnemySize);
    for (int i = 0; i < m_EnemySize; i++)
    {
        m_ZombieModelList[i] = new LNPC(m_PlayerModel.get());
        m_ZombieModelList[i]->m_pModel = LFbxMgr::GetInstance().GetPtr(L"Zombie.fbx");
        m_ZombieModelList[i]->CreateBoneBuffer();
        m_ZombieModelList[i]->FSM(FSMType::ENEMY);

        m_ZombieModelList[i]->m_matControl._41 = i * 500;
        m_ZombieModelList[i]->m_matControl._43 = i * 500;
    }

    m_ModelCamera->SetTarget(m_PlayerModel.get());

    // 맵 오브젝트 예시
    //mapObj = LFbxMgr::GetInstance().Load(L"../../res/map/Mountain.fbx", L"../../res/hlsl/ShadowMap.hlsl");
    //m_MapModel = std::make_shared<LModel>();
    //m_MapModel->SetLFbxObj(mapObj);
    //m_MapModel->CreateBoneBuffer();
    //m_MapModel->m_matControl._11 = 500.f;
    //m_MapModel->m_matControl._22 = 0.f;
    //m_MapModel->m_matControl._33 = 500.f;
    //m_MapModel->m_matControl._42 = -0.f;
    m_CustomMap = std::make_shared<LHeightMap>();
    m_CustomMap->Set();
    m_CustomMap->CreateHeightMap(L"../../res/Heightmap/heightMap513.bmp");
    //LMapDesc CMapDesc = {};
    //CMapDesc.iNumCols = 513;
    //CMapDesc.iNumRows = 513;
    //CMapDesc.fCellDistance = 1.0f;
    //CMapDesc.ShaderFilePath = L"../../res/hlsl/ShadowMap.hlsl";
    //CMapDesc.TextureFilePath = L"../../res/map/baseColor.jpg";
    //m_CustomMap->Load(CMapDesc);

    LMapDesc MapDesc = {};
    MapDesc.iNumCols = m_CustomMap->m_iNumCols;
    MapDesc.iNumRows = m_CustomMap->m_iNumRows;
    MapDesc.fCellDistance = 1.0f;
    MapDesc.fScaleHeight = 1.0f;
    MapDesc.ShaderFilePath = L"../../res/hlsl/ShadowMap.hlsl";
    MapDesc.TextureFilePath = L"../../res/map/basecolor.jpg";
    m_CustomMap->Load(MapDesc);

    // Shadow
    m_pQuad.Set();
    m_pQuad.SetScreenVertex(0, 0, 250, 250, TVector2(LGlobal::g_WindowWidth, LGlobal::g_WindowHeight));
    m_pQuad.Create(L"../../res/hlsl/ShadowMap.hlsl", L"../../res/map/castle.jpg");

    m_RT.Create(1024, 1024);

    m_matTexture = TMatrix(0.5f, 0.0f, 0.0f, 0.0f
        , 0.0f, -0.5f, 0.0f, 0.0f
        , 0.0f, 0.0f, 1.0f, 0.0f
        , 0.5f, 0.5f, 0.0f, 1.0f);

    // Collision
    std::wstring head = L"Head";
    std::wstring root = L"root";
    TMatrix Head = m_ZombieModelList[0]->m_pModel->m_NameMatrixMap[0][head];
    TMatrix Root = m_ZombieModelList[0]->m_pModel->m_NameMatrixMap[0][root];

    m_Select = new LSelect;
    m_obbBoxList.resize(m_EnemySize);
    m_BoxList.resize(m_EnemySize);
    for (int i = 0; i < m_EnemySize; i++)
    {
        m_obbBoxList[i] = new LBox;
        m_obbBoxList[i]->Set();
        m_BoxList[i] = new T_BOX;
        m_BoxList[i]->vMax = TVector3(20.0f, Head._42, 30.0f) * 0.2f;
        m_BoxList[i]->vMin = TVector3(-20.0f, Root._42, -5.0f) * 0.2f;
        m_BoxList[i]->vCenter = (m_BoxList[i]->vMax + m_BoxList[i]->vMin);
        m_BoxList[i]->vCenter.x /= 2.0f;
        m_BoxList[i]->vCenter.y /= 2.0f;
        m_BoxList[i]->vCenter.z /= 2.0f;

        m_BoxList[i]->fExtent[0] = fabs(m_BoxList[i]->vCenter.x - m_BoxList[i]->vMax.x);
        m_BoxList[i]->fExtent[1] = fabs(m_BoxList[i]->vCenter.y - m_BoxList[i]->vMax.y);
        m_BoxList[i]->fExtent[2] = fabs(m_BoxList[i]->vCenter.z - m_BoxList[i]->vMax.z);
        m_BoxList[i]->vAxis[0] = TVector3(1.0f, 0.0f, 0.0f);
        m_BoxList[i]->vAxis[1] = TVector3(0.0f, 1.0f, 0.0f);
        m_BoxList[i]->vAxis[2] = TVector3(0.0f, 0.0f, 1.0f);

        TMatrix matScale;
        D3DXMatrixScaling(&matScale, m_BoxList[i]->fExtent[0], m_BoxList[i]->fExtent[1], m_BoxList[i]->fExtent[2]);
        m_obbBoxList[i]->m_matWorld = matScale;
        m_obbBoxList[i]->m_matWorld._41 = m_BoxList[i]->vCenter.x;
        m_obbBoxList[i]->m_matWorld._42 = m_BoxList[i]->vCenter.y;
        m_obbBoxList[i]->m_matWorld._43 = m_BoxList[i]->vCenter.z;

        m_obbBoxList[i]->CreateOBBBox(m_BoxList[i]->fExtent[0], m_BoxList[i]->fExtent[1], m_BoxList[i]->fExtent[2],
            m_BoxList[i]->vCenter, m_BoxList[i]->vAxis[0], m_BoxList[i]->vAxis[1], m_BoxList[i]->vAxis[2]);
        m_obbBoxList[i]->Create(L"../../res/hlsl/TransparentBox.hlsl", L"../../res/map/topdownmap.jpg");
    }

    return true;
}

void InGameScene::Process()
{
    m_CustomMap->Frame();

    float fHeight = m_CustomMap->GetHeight(m_PlayerModel->m_matControl._41, m_PlayerModel->m_matControl._43);
    m_PlayerModel->m_matControl._42 = fHeight + 1.0f;

    if (LInput::GetInstance().m_KeyStateOld[DIK_1] > KEY_PUSH)
    {
        LGlobal::g_pMainCamera = m_DebugCamera.get();
    }

    if (LInput::GetInstance().m_KeyStateOld[DIK_2] > KEY_PUSH)
    {
        LGlobal::g_pMainCamera = m_ModelCamera.get();
    }

    m_ModelCamera->SetTargetPos(TVector3(m_PlayerModel->m_matControl._41, m_PlayerModel->m_matControl._42, m_PlayerModel->m_matControl._43));

    m_PlayerModel->Frame();
    m_PlayerModel->Process();

    m_GunModel->Frame();

    for (int i = 0; i < m_EnemySize; i++)
    {
        fHeight = m_CustomMap->GetHeight(m_ZombieModelList[i]->m_matControl._41, m_ZombieModelList[i]->m_matControl._43);
        m_ZombieModelList[i]->m_matControl._42 = fHeight + 1.0f;
        m_ZombieModelList[i]->Frame();
        m_ZombieModelList[i]->Process();
    }

    if (m_GunModel->m_pModel != nullptr && m_PlayerModel->m_pActionModel != nullptr)
    {
        if (m_PlayerModel->m_pActionModel->m_iEndFrame <= int(m_PlayerModel->m_fCurrentAnimTime)) return;

        m_GunModel->m_pModel->m_matSocket = m_PlayerModel->m_pActionModel->m_NameMatrixMap[int(m_PlayerModel->m_fCurrentAnimTime)][m_GunModel->m_ParentBoneName];

        m_GunModel->m_matControl = m_GunModel->m_pModel->m_matScale * m_GunModel->m_pModel->m_matRotation * m_GunModel->m_pModel->m_matSocket
            * m_GunModel->m_pModel->m_matTranslation * m_PlayerModel->m_matControl;
    }

    // 맵 오브젝트 예시
    //m_MapModel->Frame();

    m_Select->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);

    for (int i = 0; i < m_EnemySize; i++)
    {
        m_obbBoxList[i]->Frame();
        m_obbBoxList[i]->CreateOBBBox(m_BoxList[i]->fExtent[0], m_BoxList[i]->fExtent[1], m_BoxList[i]->fExtent[2],
            { m_obbBoxList[i]->m_matWorld._41, m_obbBoxList[i]->m_matWorld._42, m_obbBoxList[i]->m_matWorld._43 },
            m_BoxList[i]->vAxis[0], m_BoxList[i]->vAxis[1], m_BoxList[i]->vAxis[2]);
    }
    UIManager::GetInstance().Frame();
}

void InGameScene::Render()
{
    LGlobal::g_pImmediateContext->OMSetDepthStencilState(LGlobal::g_pDepthStencilStateDisable.Get(), 1);
    m_SkyBox->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
    m_SkyBox->Render();
    LGlobal::g_pImmediateContext->OMSetDepthStencilState(LGlobal::g_pDepthStencilState.Get(), 1);

    TVector4 vClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    if (m_RT.Begin(vClearColor))
    {
        TVector3 vLightPos = TVector3(-10000, 10000, -10000);
        //-----------------------------------------------------
        // 1패스:그림자맵 생성  
        //-----------------------------------------------------		
        TVector3 vEye = vLightPos;
        TVector3 vLookat = TVector3(0, 0, 0);
        TVector3 vUp = TVector3(0.0f, 1.0f, 0.0f);
        D3DXMatrixLookAtLH(&m_matViewLight, &vEye, &vLookat, &vUp);
        D3DXMatrixOrthoOffCenterLH(&m_matProjLight, -512 / 2, 512 / 2, -512 / 2, 512 / 2, 0.0f, 100.0f);
        RenderShadow(&m_PlayerModel->m_matControl, &m_matShadow, &m_matViewLight, &m_matProjLight);
        m_RT.End();
    }

    RenderObject();
    m_GunModel->Render();

    for (auto zombie : m_ZombieModelList)
    {
        zombie->Render();
    }

    // map
    //m_MapModel->m_pModel->m_DrawList[0]->SetMatrix(&m_MapModel->m_matControl, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
    //m_CBmatShadow.g_matShadow = m_MapModel->m_matControl * m_matViewLight * m_matProjLight * m_matTexture;
    //D3DXMatrixTranspose(&m_CBmatShadow.g_matShadow, &m_CBmatShadow.g_matShadow);
    //LGlobal::g_pImmediateContext->UpdateSubresource(m_pCBShadow.Get(), 0, NULL, &m_CBmatShadow, 0, 0);
    //LGlobal::g_pImmediateContext->VSSetConstantBuffers(1, 1, m_pCBShadow.GetAddressOf());
    //m_MapModel->m_pModel->m_DrawList[0]->PreRender();
    //ID3D11ShaderResourceView* pSRV[] = { m_RT.m_pSRV.Get() };
    //LGlobal::g_pImmediateContext->PSSetShaderResources(1, 1, pSRV);
    //m_MapModel->m_pModel->m_DrawList[0]->PostRender();
    m_CustomMap->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
    m_CustomMap->Render();

    // Shadow
   /* m_pQuad.SetMatrix(NULL, NULL, NULL);
    m_pQuad.PreRender();
    {
        LGlobal::g_pImmediateContext->PSSetShaderResources(0, 1, m_RT.m_pSRV.GetAddressOf());
    }
    m_pQuad.PostRender();*/

    // Collision
    for (int i = 0; i < m_EnemySize; i++)
    {
        TMatrix zombieTranslation;
        zombieTranslation.Translation(TVector3(m_ZombieModelList[i]->m_matControl._41, m_ZombieModelList[i]->m_matControl._42, m_ZombieModelList[i]->m_matControl._43));
        m_obbBoxList[i]->SetMatrix(&zombieTranslation, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
        m_obbBoxList[i]->Render();

        if (LInput::GetInstance().m_MouseState[0])
        {
            if (m_Select->ChkOBBToRay(&m_obbBoxList[i]->m_Box))
            {
                m_ZombieModelList[i]->IsTakeDamage = true;
                std::string boxintersect = "박스와 직선의 충돌, 교점 = (" + std::to_string(m_Select->m_vIntersection.x) + "," + std::to_string(m_Select->m_vIntersection.y) + "," + std::to_string(m_Select->m_vIntersection.z) + ")";
                //MessageBoxA(0, boxintersect.c_str(), 0, MB_OK);
            }
        }
    }

    std::wstring textState = L"InGameScene";
    LWrite::GetInstance().AddText(textState, 320.0f, 500.0f, { 1.0f, 1.0f, 1.0f, 1.0f });

    if (LInput::GetInstance().m_KeyStateOld[DIK_ESCAPE] == KEY_PUSH)
    {
        Release();
        m_pOwner->SetTransition(Event::GOENDSCENE);
        return;
    }

    if (LInput::GetInstance().m_KeyStateOld[DIK_ESCAPE] == KEY_PUSH)
    {
        Release();
        m_pOwner->SetTransition(Event::GOMAINSCENE);
        return;
    }
    UIManager::GetInstance().Render();
}

void InGameScene::Release()
{

}

void InGameScene::RenderObject()
{
    m_PlayerModel->m_pModel->m_DrawList[0]->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
    m_CBmatShadow.g_matShadow = m_PlayerModel->m_matControl * m_matViewLight * m_matProjLight * m_matTexture;
    D3DXMatrixTranspose(&m_CBmatShadow.g_matShadow, &m_CBmatShadow.g_matShadow);
    LGlobal::g_pImmediateContext->UpdateSubresource(m_pCBShadow.Get(), 0, NULL, &m_CBmatShadow, 0, 0);
    LGlobal::g_pImmediateContext->VSSetConstantBuffers(1, 1, m_pCBShadow.GetAddressOf());
    m_PlayerModel->m_pModel->m_DrawList[0]->PreRender();
    ID3D11ShaderResourceView* pSRV[] = { m_RT.m_pSRV.Get() };
    LGlobal::g_pImmediateContext->PSSetShaderResources(1, 1, pSRV);
    m_PlayerModel->Render();
}

void InGameScene::RenderShadow(TMatrix* matWorld, TMatrix* matShadow,
    TMatrix* matView, TMatrix* matProj)
{
    TMatrix matWorldShadow = (*matWorld) * (*matShadow);
    m_PlayerModel->m_pModel->m_DrawList[0]->SetMatrix(&matWorldShadow, matView, matProj);
    m_PlayerModel->Render();
}

void InGameScene::CreateShadowConstantBuffer()
{
    D3D11_BUFFER_DESC bufferDesc{};
    ZeroMemory(&bufferDesc, sizeof(bufferDesc));

    bufferDesc.ByteWidth = sizeof(SHADOW_CONSTANT_BUFFER);
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    HRESULT hr = LGlobal::g_pDevice->CreateBuffer(
        &bufferDesc,
        nullptr,
        m_pCBShadow.GetAddressOf());

    if (FAILED(hr))
    {
        MessageBoxA(NULL, "Create ConstantBuffer Error", "Error Box", MB_OK);
        return;
    }
}

InGameScene::InGameScene(LScene* parent) : SceneState(parent)
{
    Init();
}
InGameScene::~InGameScene() {}