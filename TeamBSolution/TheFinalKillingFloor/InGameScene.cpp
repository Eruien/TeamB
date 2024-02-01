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

    m_MinimapCamera = std::make_shared<LCamera>();
    m_MinimapCamera->CreateLookAt({ 0.0f, 2500.0f, 0.0f }, { 0.0f, 0.0f, 1.0f });
    m_MinimapCamera->m_fCameraPitch =0.f;
    m_MinimapCamera->CreatePerspectiveFov(L_PI * 0.25f, 1.0f, 1.0f, 10000.0f);
    m_MinimapCamera->CreateOrthographic((float)256, (float)256, -1.0f, 10000.0f);

    m_playerIcon = make_shared<KObject>();
    m_playerIcon->Init();
    m_playerIcon->SetPos({ 0, 0, -1 });
    m_playerIcon->SetScale({100,100, 1});
  
    m_playerIcon->Create(L"../../res/hlsl/CustomizeMap.hlsl", L"../../res/ui/PlayerIcon.png");

    CreateShadowConstantBuffer();

    m_SkyBox = std::make_shared<LSkyBox>();
    m_SkyBox->Set();
    m_SkyBox->Create(L"../../res/hlsl/SkyBox.hlsl", L"../../res/sky/grassenvmap1024.dds");

    
    LCharacterIO::GetInstance().CharacterRead(L"../../res/UserFile/Character/Zombie.bin", L"../../res/hlsl/CharacterShaderInAnimationData.hlsl");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Zombie_Attack_Anim.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Zombie_Death.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Zombie_TakeDamage.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Zombie_Walk_Lock.bin");

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
    m_ZombieModelList[0]->ComputeOffset();

    LCharacterIO::GetInstance().CharacterRead(L"../../res/UserFile/Character/army3.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Fire_Rifle_Ironsights.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Reload_Rifle_Ironsights.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Sprint_Fwd_Rifle.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Walk_Fwd_Rifle_Ironsights.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Idle_Rifle_Ironsights.bin");
    LCharacterIO::GetInstance().ItemRead(L"../../res/UserFile/Item/Assault_Rifle_A.bin");

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

    for (int i = 0; i < m_EnemySize; i++)
    {
        m_ZombieModelList[i]->m_Player = m_PlayerModel.get();
        m_ZombieModelList[i]->SetAnimationArrayTexture();
        m_ZombieModelList[i]->SetAnimationArraySRV();
        m_ZombieModelList[i]->CreateCurrentFrameBuffer();
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
    //m_CustomMap->CreateHeightMap(L"../../res/Heightmap/Heightmap_04_Plains.png");
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
    MapDesc.fCellDistance = 4.0f;
    MapDesc.fScaleHeight = 0.4f;
    MapDesc.ShaderFilePath = L"../../res/hlsl/ShadowMap.hlsl";
    MapDesc.TextureFilePath = L"../../res/map/aerial_grass_rock_diff_8k.jpg"; 
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
    std::wstring shoulder = L"RightShoulder";
    std::wstring hand = L"RightHand";
 
    TMatrix Head = m_PlayerModel->m_pModel->m_NameMatrixMap[0][head];
    TMatrix Root = m_PlayerModel->m_pModel->m_NameMatrixMap[0][root];
  
    m_PlayerModel->SetOBBBox({ -30.0f, Root._42, -20.0f }, { 30.0f, Head._42, 30.0f }, 0.2f);
  
    Head = m_ZombieModelList[0]->m_pModel->m_NameMatrixMap[0][head];
    Root = m_ZombieModelList[0]->m_pModel->m_NameMatrixMap[0][root];
    TMatrix RightShoulder = m_ZombieModelList[0]->m_pModel->m_NameMatrixMap[0][shoulder];
    TMatrix RightHand = m_ZombieModelList[0]->m_pModel->m_NameMatrixMap[0][hand];

    m_Select = new LSelect;

    for (int i = 0; i < m_EnemySize; i++)
    {
        m_ZombieModelList[i]->SetOBBBox({ -20.0f, Root._42, -5.0f }, { 20.0f, Head._42, 30.0f }, 0.2f);
        m_ZombieModelList[i]->SetOBBBoxRightHand({ RightHand._41, RightHand._42, -40.0f }, { RightShoulder._41, RightShoulder._42, 40.0f }, 0.2f);
    }

    //Minimap
    m_ShapeMinimap.Set();
    m_ShapeMinimap.SetScreenVertex(0, 0, 256, 256, TVector2(LGlobal::g_WindowWidth, LGlobal::g_WindowHeight));
    m_ShapeMinimap.Create(L"../../res/hlsl/CustomizeMap.hlsl", L"../../res/ui/Hud_Box_128x64.png");
    m_rtMinimap.Create(256, 256);

    m_enemyHp = make_shared<KObject>();
    m_enemyHp->Init();
    m_enemyHp->Create(L"../../res/hlsl/CustomizeMap.hlsl", L"../../res/ui/hp_bar.png");
    m_enemyHp->SetPos({ 0, 0, 0 });
    m_enemyHp->SetScale({ 12,2,1 });

    return true;
}

void InGameScene::Process()
{
    // 맵 오브젝트 예시
    //m_MapModel->Frame();
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

    m_ModelCamera->SetTargetPos(TVector3(m_PlayerModel->m_matControl._41 + 20, m_PlayerModel->m_matControl._42 + 15, m_PlayerModel->m_matControl._43));

    m_PlayerModel->Frame();
    m_PlayerModel->Process();

    m_GunModel->Frame();

    for (int i = 0; i < m_EnemySize; i++)
    {
        m_ZombieModelList[i]->IsMovable = true;
    }
    for (int i = 0; i < m_EnemySize; i++)
    {
        fHeight = m_CustomMap->GetHeight(m_ZombieModelList[i]->m_matControl._41, m_ZombieModelList[i]->m_matControl._43);
        m_ZombieModelList[i]->m_matControl._42 = fHeight + 1.0f;
        m_ZombieModelList[i]->Process();
        m_ZombieModelList[i]->Frame();
        // collision check
        for (int j = i + 1; j < m_EnemySize; j++)
        {
            if (m_ZombieModelList[i]->m_OBBBox.CollisionCheck(&m_ZombieModelList[j]->m_OBBBox))
            {
                m_ZombieModelList[i]->IsMovable = false;
                m_ZombieModelList[j]->IsMovable = false;
                float offsetX = m_ZombieModelList[i]->m_OBBBox.m_Box.vCenter.x - m_ZombieModelList[i]->m_OBBBox.m_Box.vCenter.x;
                float offsetZ = m_ZombieModelList[i]->m_OBBBox.m_Box.vCenter.z - m_ZombieModelList[i]->m_OBBBox.m_Box.vCenter.z;

                m_ZombieModelList[i]->m_matControl._41 += offsetX*0.2;
                m_ZombieModelList[i]->m_matControl._43 += offsetZ*0.2;
            }
            
        }
        

    }
    // 빌보드


    m_enemyHp->SetPos({ m_ZombieModelList[0]->m_matControl._41, m_ZombieModelList[0]->m_matControl._42 + 40,m_ZombieModelList[0]->m_matControl._43 });
    /*  LWRITE.AddText(to_wstring(m_PlayerModel->m_matControl._41), 100, 400);
      LWRITE.AddText(to_wstring(m_PlayerModel->m_matControl._41), 100, 500);*/
    TMatrix matRotation, matTrans, matScale, worldMat;
    D3DXMatrixInverse(&matRotation, nullptr, &LGlobal::g_pMainCamera->m_matView);
    matRotation._41 = 0.0f;
    matRotation._42 = 0.0f;
    matRotation._43 = 0.0f;
    matRotation._44 = 1.0f;

    D3DXMatrixTranslation(&matTrans, m_enemyHp->m_vPosition.x,
        m_enemyHp->m_vPosition.y,
        m_enemyHp->m_vPosition.z
    );

    D3DXMatrixScaling(&matScale, m_enemyHp->m_vScale.x,
        m_enemyHp->m_vScale.y,
        m_enemyHp->m_vScale.z
    );
    worldMat = matScale * matRotation * matTrans;
    m_enemyHp->SetMatrix(&worldMat, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
    m_enemyHp->Frame();
    // 빌보드 끝
    if (m_GunModel->m_pModel != nullptr && m_PlayerModel->m_pActionModel != nullptr)
    {
        if (m_PlayerModel->m_pActionModel->m_iEndFrame <= int(m_PlayerModel->m_fCurrentAnimTime)) return;

        m_GunModel->m_pModel->m_matSocket = m_PlayerModel->m_pActionModel->m_NameMatrixMap[int(m_PlayerModel->m_fCurrentAnimTime)][m_GunModel->m_ParentBoneName];

        m_GunModel->m_matControl = m_GunModel->m_pModel->m_matScale * m_GunModel->m_pModel->m_matRotation * m_GunModel->m_pModel->m_matSocket
            * m_GunModel->m_pModel->m_matTranslation * m_PlayerModel->m_matControl;
    }


    // collision
    m_Select->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);

    m_PlayerModel->m_OBBBox.Frame();
    m_PlayerModel->m_OBBBox.CreateOBBBox(m_PlayerModel->m_SettingBox.fExtent[0], m_PlayerModel->m_SettingBox.fExtent[1], m_PlayerModel->m_SettingBox.fExtent[2],
        { m_PlayerModel->m_OBBBox.m_matWorld._41, m_PlayerModel->m_OBBBox.m_matWorld._42, m_PlayerModel->m_OBBBox.m_matWorld._43 },
        m_PlayerModel->m_SettingBox.vAxis[0], m_PlayerModel->m_SettingBox.vAxis[1], m_PlayerModel->m_SettingBox.vAxis[2]);

    for (int i = 0; i < m_EnemySize; i++)
    {
        m_ZombieModelList[i]->m_OBBBox.Frame();
        m_ZombieModelList[i]->m_OBBBox.CreateOBBBox(m_ZombieModelList[i]->m_SettingBox.fExtent[0], m_ZombieModelList[i]->m_SettingBox.fExtent[1], m_ZombieModelList[i]->m_SettingBox.fExtent[2],
            { m_ZombieModelList[i]->m_OBBBox.m_matWorld._41, m_ZombieModelList[i]->m_OBBBox.m_matWorld._42, m_ZombieModelList[i]->m_OBBBox.m_matWorld._43 },
            m_ZombieModelList[i]->m_SettingBox.vAxis[0], m_ZombieModelList[i]->m_SettingBox.vAxis[1], m_ZombieModelList[i]->m_SettingBox.vAxis[2]);

        m_ZombieModelList[i]->m_OBBBoxRightHand.Frame();
        m_ZombieModelList[i]->m_OBBBoxRightHand.CreateOBBBox(m_ZombieModelList[i]->m_SettingBoxRightHand.fExtent[0], m_ZombieModelList[i]->m_SettingBoxRightHand.fExtent[1], m_ZombieModelList[i]->m_SettingBoxRightHand.fExtent[2],
            { m_ZombieModelList[i]->m_OBBBoxRightHand.m_matWorld._41, m_ZombieModelList[i]->m_OBBBoxRightHand.m_matWorld._42, m_ZombieModelList[i]->m_OBBBoxRightHand.m_matWorld._43 },
            m_ZombieModelList[i]->m_SettingBoxRightHand.vAxis[0], m_ZombieModelList[i]->m_SettingBoxRightHand.vAxis[1], m_ZombieModelList[i]->m_SettingBoxRightHand.vAxis[2]);

       /* if (m_ZombieModelList[i]->m_pActionModel->m_iEndFrame <= int(m_ZombieModelList[i]->m_fCurrentAnimTime)) return;

        TMatrix zombieRightHandSocket = m_ZombieModelList[i]->m_pActionModel->m_NameMatrixMap[int(m_ZombieModelList[i]->m_fCurrentAnimTime)][L"RightHand"];

        m_ZombieModelList[i]->m_OBBBoxRightHand.m_matWorld = zombieRightHandSocket;*/
    }
    UIManager::GetInstance().Frame();
}

void InGameScene::Render()
{
    LGlobal::g_pImmediateContext->OMSetDepthStencilState(LGlobal::g_pDepthStencilStateDisable.Get(), 1);
    m_SkyBox->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
    m_SkyBox->Render();
    LGlobal::g_pImmediateContext->OMSetDepthStencilState(LGlobal::g_pDepthStencilState.Get(), 1);

    m_enemyHp->Render();

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
        zombie->AniRender();
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

    TMatrix playerTranslation;
    playerTranslation.Translation(TVector3(m_PlayerModel->m_matControl._41, m_PlayerModel->m_matControl._42 + m_PlayerModel->m_SettingBox.vCenter.y, m_PlayerModel->m_matControl._43));
    m_PlayerModel->m_OBBBox.SetMatrix(&playerTranslation, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
    m_PlayerModel->m_OBBBox.Render();

    for (int i = 0; i < m_EnemySize; i++)
    {
        TMatrix zombieTranslation;
        zombieTranslation.Translation(TVector3(m_ZombieModelList[i]->m_matControl._41, m_ZombieModelList[i]->m_matControl._42 + m_ZombieModelList[i]->m_SettingBox.vCenter.y, m_ZombieModelList[i]->m_matControl._43));
        m_ZombieModelList[i]->m_OBBBox.SetMatrix(&zombieTranslation, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
        m_ZombieModelList[i]->m_OBBBox.Render();

        TMatrix zombieRightHandSocket;
        TMatrix matRightHand;
        if (m_ZombieModelList[i]->m_pActionModel != nullptr)
        {
            if (m_ZombieModelList[i]->m_pActionModel->m_iEndFrame >= int(m_ZombieModelList[i]->m_fCurrentAnimTime))
            {
                int currentFrame = max(m_ZombieModelList[i]->m_fCurrentAnimTime - m_ZombieModelList[i]->m_pActionModel->m_iStartFrame, 0);
                zombieRightHandSocket = m_ZombieModelList[i]->m_pActionModel->m_NameMatrixMap[int(currentFrame)][L"RightHand"];
            }
        }
        
        matRightHand = zombieRightHandSocket * m_ZombieModelList[i]->m_matControl;
        m_ZombieModelList[i]->m_OBBBoxRightHand.SetMatrix(&matRightHand, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
        m_ZombieModelList[i]->m_OBBBoxRightHand.Render();

        if (LInput::GetInstance().m_MouseState[0])
        {
            if (m_Select->ChkOBBToRay(&m_ZombieModelList[i]->m_OBBBox.m_Box))
            {
                if (m_PlayerModel->IsShoot)
                {
                    m_ZombieModelList[i]->IsTakeDamage = true;
                }
                
                //std::string boxintersect = "박스와 직선의 충돌, 교점 = (" + std::to_string(m_Select->m_vIntersection.x) + "," + std::to_string(m_Select->m_vIntersection.y) + "," + std::to_string(m_Select->m_vIntersection.z) + ")";
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
    //UI
    UIManager::GetInstance().Render();
    m_ShapeMinimap.SetMatrix(NULL, NULL, NULL);



    //미니맵
    if (m_rtMinimap.Begin(vClearColor))
    {
        m_CustomMap->SetMatrix(nullptr, &m_MinimapCamera->m_matView, &m_MinimapCamera->m_matProj);
        m_CustomMap->Render();

        LWRITE.AddText(to_wstring(m_PlayerModel->m_matControl._41), 100, 400);
        LWRITE.AddText(to_wstring(m_PlayerModel->m_matControl._42), 100, 500);
        LWRITE.AddText(to_wstring(m_PlayerModel->m_matControl._43), 100, 600);
        LWRITE.AddText(to_wstring(LINPUT.m_vOffset.y), 500, 600);

        m_playerIcon->m_vPosition={ m_PlayerModel->m_matControl._41*(float(LGlobal::g_WindowWidth) / 2048.f) ,0, m_PlayerModel->m_matControl._43 * (float(LGlobal::g_WindowHeight) / 2048.0f) };
        m_playerIcon->m_vRotation.z = -m_ModelCamera->m_fCameraYaw;
        
        m_playerIcon->SetMatrix(nullptr, &m_MinimapCamera->m_matView, &m_MinimapCamera->m_matOrthoProjection);
        m_playerIcon->Frame();
        m_playerIcon->Render();
        //m_PlayerModel

        m_rtMinimap.End();
    }

    m_ShapeMinimap.PreRender();
   {
       LGlobal::g_pImmediateContext->PSSetShaderResources(0, 1, m_rtMinimap.m_pSRV.GetAddressOf());
   }
   m_ShapeMinimap.PostRender();


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
    m_PlayerModel->m_pModel->m_DrawList[0]->SetMatrix( &matWorldShadow, matView, matProj);
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

