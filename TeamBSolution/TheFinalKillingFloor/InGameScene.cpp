#include "InGameScene.h"
#include "LGlobal.h"
#include "LInput.h"
#include "LWrite.h"
#include "UIManager.h"
#include "LCharacterIO.h"
#include "LAnimationIO.h"
#include "LExportIO.h"
static bool Init_2 = true;
bool InGameScene::Init()
{

    InitializeObjects();
    InitializeSkyBox();
    InitializePlayerIcon();
    InitializeBloodSplatters();
    InitializeMap();
    InitializeWalls();
    InitializeBullets();
    InitializeLighting();
    InitializeShadow();
    InitializeTrees();
    InitializeGrasses();
    InitializeMinimap();
    InitializeMuzzleFlash();

    InitializeOBBBox();


    m_PlayerFirstSpawnPos = { LGlobal::g_PlayerModel->m_matControl._41, LGlobal::g_PlayerModel->m_matControl._42, LGlobal::g_PlayerModel->m_matControl._43 };

    return true;
}
void InGameScene::Process()
{
    ProcessMuzzleFlash();
    ProcessBloodSplatter();
    CheckPlayerDeath();
    PlayInGameSound();
    UpdateUI();
    ProcessWaveTransition();
    UpdateMapObjects();
    UpdateWallModels();
    UpdateTreeModels();
    UpdateBulletModels();
    AdjustPlayerHeight();
    SwitchCameraView();
    UpdateCameraTargetPosition();
    FramePlayerModel();
    FrameGunModel();
    UpdateZombieAndTankModels();
    HandlePlayerTreeCollisions();
    LimitPlayerMovement();
    UpdateGunModelPosition();
    FrameCollisionDetection();
    FrameUI();
    FramePointLight();
}

void InGameScene::Render()
{
    // 새로운 FPS 값을 큐에 추가
    fpsValues.push_back(1.0f / LGlobal::g_fSPF);
    // 큐의 크기가 일정 크기를 넘으면 가장 오래된 값을 제거
    if (fpsValues.size() > 100) {
        fpsValues.pop_front();
    }
    // FPS 값을 평균내어 텍스트로 변환
    float averageFPS = std::accumulate(fpsValues.begin(), fpsValues.end(), 0.0f) / fpsValues.size();
    std::wstring fpsText = std::to_wstring(int(averageFPS));
    LWrite::GetInstance().AddText(fpsText, 10.0f, 10.0f, { 1.0f, 1.0f, 1.0f, 1.0f });

    if (!m_VisibleBulletList[LGlobal::g_BulletCount])
        m_PointLight[0].m_vPosition.y = -1000.f;
    LGlobal::g_pImmediateContext->OMSetDepthStencilState(LGlobal::g_pDepthStencilStateDisable.Get(), 1);
    m_SkyBox->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
    m_SkyBox->Render();
    LGlobal::g_pImmediateContext->OMSetDepthStencilState(LGlobal::g_pDepthStencilState.Get(), 1);
    //LGlobal::g_pImmediateContext->OMSetBlendState(0, 0, 0xffffffff);
    // Light Render
    m_cbLight1.g_cAmbientMaterial[0] = TVector4(0.1f, 0.1f, 0.1f, 1);
    m_cbLight1.g_cDiffuseMaterial[0] = TVector4(1, 1, 1, 1);
    m_cbLight1.g_cSpecularMaterial[0] = TVector4(1, 1, 1, 1);
    m_cbLight1.g_cEmissionMaterial[0] = TVector4(0, 0, 0, 1);

    m_cbLight1.g_cAmbientLightColor[0] = TVector4(1, 1, 1, 1);
    m_cbLight1.g_cSpecularLightColor[0] = TVector4(1, 1, 1, 1);
    m_cbLight1.g_cDiffuseLightColor[0] = TVector4(m_PointLight[0].m_DiffuseColor.x,
        m_PointLight[0].m_DiffuseColor.y,
        m_PointLight[0].m_DiffuseColor.z, 1.0f);
    /*TMatrix matInvWorld;
    D3DXMatrixInverse(&matInvWorld, NULL, &m_PointLight[0].m_matWorld);
    D3DXMatrixTranspose(&matInvWorld, &matInvWorld);
    D3DXMatrixTranspose(&m_cbLight2.g_matInvWorld[0], &matInvWorld);*/

   /* m_cbLight2.g_vEyeDir[0].x = LGlobal::g_pMainCamera->m_vLook.x;
    m_cbLight2.g_vEyeDir[0].y = LGlobal::g_pMainCamera->m_vLook.y;
    m_cbLight2.g_vEyeDir[0].z = LGlobal::g_pMainCamera->m_vLook.z;*/
    //m_cbLight2.g_vEyeDir[0].w = 100.0f; // 강도

    m_cbLight2.g_vLightPos[0] = TVector4(m_PointLight[0].m_vPosition.x,
        m_PointLight[0].m_vPosition.y,
        m_PointLight[0].m_vPosition.z,
        m_PointLight[0].m_fRange);

    m_cbLight2.g_vLightDir[0] = TVector4(m_PointLight[0].m_vDirection.x,
        m_PointLight[0].m_vDirection.y,
        m_PointLight[0].m_vDirection.z, 1.0f);
    m_cbLight1.g_bIsZedTime = LGlobal::g_PlayerModel->IsZedTime ? 1.0f : 0.0f;
    //if (LGlobal::g_PlayerModel->IsZedTime == true)
    //{
    //    m_cbLight1.g_bIsZedTime = LGlobal::g_PlayerModel->IsZedTime;
    //}
    LGlobal::g_pImmediateContext->UpdateSubresource(m_pConstantBufferLight[0].Get(), 0, NULL, &m_cbLight1, 0, 0);
    LGlobal::g_pImmediateContext->UpdateSubresource(m_pConstantBufferLight[1].Get(), 0, NULL, &m_cbLight2, 0, 0);
    
    ID3D11Buffer* pBuffers[2];
    pBuffers[0] = m_pConstantBufferLight[0].Get();
    pBuffers[1] = m_pConstantBufferLight[1].Get();
    LGlobal::g_pImmediateContext->PSSetConstantBuffers(3, 2, pBuffers);

    //
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
        RenderShadow(&LGlobal::g_PlayerModel->m_matControl, &m_matShadow, &m_matViewLight, &m_matProjLight);
        m_RT.End();
    }

    RenderObject();
    m_GunModel->Render();
    
    m_ZombieWave->Render();

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
    static float sTime;
    sTime += LGlobal::g_fSPF;


    for (auto& wall : m_WallList)
    {
        wall->Render();
    }

    for (auto& tree : m_TreeList)
    {
        tree->Render();
    }
    // grass
    for (auto& grass : m_GrassList)
    {
        grass->Render();
    }

    // bullet
    for (int i = 0; i < m_BulletList.size(); i++)
    {
        if (m_VisibleBulletList[i])
        {
            m_BulletList[i]->Render();
        }
    }

    //muzzleFlash
    if (LInput::GetInstance().m_MouseState[0] > KEY_PUSH && LGlobal::g_BulletCount > 0 && LGlobal::g_PlayerModel->IsEndReload)
    {
        if (sTime >= LGlobal::g_PlayerModel->m_ShotDelay)
        {
            m_muzzleFlash->SetIsRender(true);

            sTime = 0;
        }
        else if (sTime + 0.05f >= LGlobal::g_PlayerModel->m_ShotDelay)
        {
            m_muzzleFlash->SetIsRender(false);
        }
        m_muzzleFlash->Render();
    }

    //blood
    for (auto& obj : m_bloodSplatter)
    {
        obj->Render();
    }


    // Shadow
   /* m_pQuad.SetMatrix(NULL, NULL, NULL);
    m_pQuad.PreRender();
    {
        LGlobal::g_pImmediateContext->PSSetShaderResources(0, 1, m_RT.m_pSRV.GetAddressOf());
    }
    m_pQuad.PostRender();*/

    // Collision
    
    TMatrix playerTranslation;
    playerTranslation.Translation(TVector3(LGlobal::g_PlayerModel->m_matControl._41, LGlobal::g_PlayerModel->m_matControl._42 + LGlobal::g_PlayerModel->m_SettingBox.vCenter.y, LGlobal::g_PlayerModel->m_matControl._43));
    LGlobal::g_PlayerModel->m_OBBBox.SetMatrix(&playerTranslation, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
    LGlobal::g_PlayerModel->m_OBBBox.Render();

    m_ZombieWave->CollisionBoxRender();

    if (LInput::GetInstance().m_MouseState[0])
    {
        for (auto& zombie : m_ZombieWave->m_EnemyMap["Zombie"])
        {
            if (m_Select->ChkOBBToRay(&zombie->m_OBBBox.m_Box))
            {
                if (LGlobal::g_PlayerModel->IsShoot && LGlobal::g_BulletCount > 0)
                {
                    if (abs(m_Select->m_vIntersection.y - zombie->m_OBBBox.m_Box.vMax.y) < 5.f)
                    {
                        zombie->IsHeadShot = true;
                    }
                    else
                    {
                        zombie->IsHeadShot = false;
                    }
                    zombie->IsTakeDamage = true;
                    m_bloodSplatter[m_crrBlood]->SetPos(m_Select->m_vIntersection + LGlobal::g_PlayerModel->m_matControl.Forward() * 150);
                    m_bloodSplatter[m_crrBlood]->GetScript<Animator>(L"Animator")->_currentKeyframeIndex = 0;
                    m_bloodSplatter[m_crrBlood]->SetIsRender(true);
                    m_crrBlood++;
                    if (m_crrBlood == m_bloodSplatter.size())
                        m_crrBlood = 0;
                }

                //std::string boxintersect = "박스와 직선의 충돌, 교점 = (" + std::to_string(m_Select->m_vIntersection.x) + "," + std::to_string(m_Select->m_vIntersection.y) + "," + std::to_string(m_Select->m_vIntersection.z) + ")";
                //MessageBoxA(0, boxintersect.c_str(), 0, MB_OK);
            }
        }
    }

    if (LInput::GetInstance().m_MouseState[0])
    {
        for (auto& tank : m_ZombieWave->m_EnemyMap["Tank"])
        {
            if (m_Select->ChkOBBToRay(&tank->m_OBBBox.m_Box))
            {
                if (LGlobal::g_PlayerModel->IsShoot && LGlobal::g_BulletCount > 0)
                {
                    if (abs(m_Select->m_vIntersection.y - tank->m_OBBBox.m_Box.vMax.y) < 7.f)
                    {
                        tank->IsHeadShot = true;
                    }
                    else
                    {
                        tank->IsHeadShot = false;
                    }
                    tank->IsTakeDamage = true;

                    m_bloodSplatter[m_crrBlood]->SetPos(m_Select->m_vIntersection);
                    m_bloodSplatter[m_crrBlood]->GetScript<Animator>(L"Animator")->_currentKeyframeIndex = 0;
                    m_bloodSplatter[m_crrBlood]->SetIsRender(true);
                    m_crrBlood++;
                    if (m_crrBlood == m_bloodSplatter.size())
                        m_crrBlood = 0;
                }

                //std::string boxintersect = "박스와 직선의 충돌, 교점 = (" + std::to_string(m_Select->m_vIntersection.x) + "," + std::to_string(m_Select->m_vIntersection.y) + "," + std::to_string(m_Select->m_vIntersection.z) + ")";
                //MessageBoxA(0, boxintersect.c_str(), 0, MB_OK);
            }
        }
    }
   
    std::wstring textState = L"InGameScene";
    //LWrite::GetInstance().AddText(textState, 320.0f, 500.0f, { 1.0f, 1.0f, 1.0f, 1.0f });

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
    //
    //
    //
    //
    if (m_rtMinimap.Begin(vClearColor))
    {
        m_CustomMap->SetMatrix(nullptr, &m_MinimapCamera->m_matView, &m_MinimapCamera->m_matProj);
        m_CustomMap->Render();

       
 
        float offset = (   float(LGlobal::g_WindowWidth)/ float(LGlobal::g_WindowHeight))-0.3f;
        //256==렌더타켓 사이즈, 2048 = 맵 사이즈
        m_playerIcon->m_vPosition={ LGlobal::g_PlayerModel->m_matControl._41 * (256.0f / 2048.0f) ,0, LGlobal::g_PlayerModel->m_matControl._43 * (256.0f / 2048.0f) * offset };
        m_playerIcon->m_vRotation.z =- LGlobal::g_pMainCamera->m_fCameraYaw;
     


        m_playerIcon->SetMatrix(nullptr, &m_MinimapPosCamera->m_matView, &m_MinimapPosCamera->m_matOrthoProjection);
        m_playerIcon->Frame();
        m_playerIcon->Render();

        for (auto obj : m_ZombieWave->m_EnemyMap["Zombie"])
        {
            obj->m_minimapMarker->m_vPosition = { obj->m_matControl._41 * (256.0f / 2048.0f) ,0, obj->m_matControl._43 * (256.0f / 2048.0f) * offset };
            //obj->m_minimapMarker->m_vRotation.z = -m_ModelCamera->m_fCameraYaw;
            obj->m_minimapMarker->SetMatrix(nullptr, &m_MinimapPosCamera->m_matView, &m_MinimapPosCamera->m_matOrthoProjection);
            obj->m_minimapMarker->Frame();
            obj->RenderMark();
        }

        for (auto obj : m_ZombieWave->m_EnemyMap["Tank"])
        {
            obj->m_minimapMarker->m_vPosition = { obj->m_matControl._41 * (256.0f / 2048.0f) ,0, obj->m_matControl._43 * (256.0f / 2048.0f) * offset };
            //obj->m_minimapMarker->m_vRotation.z = -m_ModelCamera->m_fCameraYaw;
            obj->m_minimapMarker->SetMatrix(nullptr, &m_MinimapPosCamera->m_matView, &m_MinimapPosCamera->m_matOrthoProjection);
            obj->m_minimapMarker->Frame();
            obj->RenderMark();
        }


        //LGlobal::g_PlayerModel

        m_rtMinimap.End();
    }

    m_ShapeMinimap.PreRender();
   {
       LGlobal::g_pImmediateContext->PSSetShaderResources(0, 1, m_rtMinimap.m_pSRV.GetAddressOf());
   }
   m_ShapeMinimap.PostRender();

   Release();

   if (IsEndGame)
   {
       LInput::GetInstance().CursorChange();
       UIManager::GetInstance().Load(L"EndScene.xml");
       m_pOwner->SetTransition(Event::GOENDSCENE);
   }

   if (LInput::GetInstance().m_MouseState[1] == KEY_PUSH)
   {
       LScene::GetInstance().m_pActionList[State::INGAMESCENE]->Retry();
     //  Retry();
       
   }
}

void InGameScene::Retry()
{
    LGlobal::g_IngameSound->Play(true);
    IsEndGame = false;
    DeleteCurrentObject();
    PlayerInit();
    LGlobal::g_BulletCount = 30;
    m_ZombieWave->m_CurrentWave = 0;
    NextWave();
    Init_2 = true;
    UpdateUI();
    UIManager::GetInstance().GetUIObject(L"HPbar")->GetScript<HpBar>(L"HpBar")->UpdateHp();
}

void InGameScene::DeleteCurrentObject()
{
    for (auto iter = m_ZombieWave->m_EnemyMap["Zombie"].begin(); iter != m_ZombieWave->m_EnemyMap["Zombie"].end();)
    {
        iter = m_ZombieWave->m_EnemyMap["Zombie"].erase(iter);
    }

    for (auto iter = m_ZombieWave->m_EnemyMap["Tank"].begin(); iter != m_ZombieWave->m_EnemyMap["Tank"].end();)
    {
        iter = m_ZombieWave->m_EnemyMap["Tank"].erase(iter);
    }

    m_ZombieWave->m_EnemyMap["LNPC"].clear();

    delete LGlobal::g_PlayerModel;
    LGlobal::g_PlayerModel = nullptr;
}

void InGameScene::Release()
{
    for (auto iter = m_ZombieWave->m_EnemyMap["Zombie"].begin(); iter != m_ZombieWave->m_EnemyMap["Zombie"].end();)
    {
        if ((*iter)->IsDead)
        {
            iter = m_ZombieWave->m_EnemyMap["Zombie"].erase(iter);
            UIManager::GetInstance().GetUIObject(L"EnemyCount")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(m_ZombieWave->m_EnemyMap["Zombie"].size());
            LGlobal::g_PlayerModel->m_ZedTimeCount += 1;
        }
        else
        {
            iter++;
        }

        if (m_ZombieWave->m_EnemyMap["Zombie"].size() <= 0)
        {
            IsNextWave = true;
        }
    }

    for (auto iter = m_ZombieWave->m_EnemyMap["Tank"].begin(); iter != m_ZombieWave->m_EnemyMap["Tank"].end();)
    {
        if ((*iter)->IsDead)
        {
            iter = m_ZombieWave->m_EnemyMap["Tank"].erase(iter);
            LGlobal::g_PlayerModel->m_ZedTimeCount += 1;
        }
        else
        {
            iter++;
        }
    }

    if (m_ZombieWave->m_EnemyMap["Tank"].size() <= 0)
    {
        IsReleaseTank = true;
    }
}

void InGameScene::SoundInit()
{
    LGlobal::g_IngameSound = LSoundMgr::GetInstance().Load(L"../../res/sound/InGameMusic.mp3");
    LGlobal::g_EffectSound1 = LSoundMgr::GetInstance().Load(L"../../res/sound/fire3.wav");
    LGlobal::g_EffectSound2 = LSoundMgr::GetInstance().Load(L"../../res/sound/step1.wav");
    LGlobal::g_SteamPackSound = LSoundMgr::GetInstance().Load(L"../../res/sound/SteamPack.wav");
    LGlobal::g_ZedTimeStart = LSoundMgr::GetInstance().Load(L"../../res/sound/ZedTimeFirst.mp3");
    LGlobal::g_ZedTimeEnd = LSoundMgr::GetInstance().Load(L"../../res/sound/ZedTimeLast.mp3");
    LGlobal::g_PlayerHitSound = LSoundMgr::GetInstance().Load(L"../../res/sound/Attacked.WAV");
    LGlobal::g_HeadShotSound = LSoundMgr::GetInstance().Load(L"../../res/sound/headshot.mp3");
    LGlobal::g_KillSound = LSoundMgr::GetInstance().Load(L"../../res/sound/killsound.mp3");
}

void InGameScene::CameraInit()
{
    m_DebugCamera = std::make_shared<LDebugCamera>();
    m_DebugCamera->CreateLookAt({ 0.0f, 200.0f, -100.0f }, { 0.0f, 0.0f, 1.0f });
    m_DebugCamera->CreatePerspectiveFov(L_PI * 0.25, (float)LGlobal::g_WindowWidth / (float)LGlobal::g_WindowHeight, 1.0f, 10000.0f);

    m_ModelCamera = std::make_shared<LModelCamera>();
    m_ModelCamera->SetTargetPos(TVector3(0.0f, 0.0f, 0.0f));
    m_ModelCamera->CreatePerspectiveFov(L_PI * 0.25, (float)LGlobal::g_WindowWidth / (float)LGlobal::g_WindowHeight, 1.0f, 10000.0f);
    m_ModelCamera->m_fRadius = 100.0f;

    m_BackViewCamera = std::make_shared<LBackView>();
    m_BackViewCamera->CreateLookAt({ 0.0f, 200.0f, -100.0f }, { 0.0f, 0.0f, 1.0f });
    m_BackViewCamera->CreatePerspectiveFov(L_PI * 0.25, (float)LGlobal::g_WindowWidth / (float)LGlobal::g_WindowHeight, 1.0f, 10000.0f);
    LGlobal::g_pMainCamera = m_BackViewCamera.get();

    m_MinimapCamera = std::make_shared<LCamera>();
    m_MinimapCamera->CreateLookAt({ 0.0f, 6000.0f, 0.0f }, { 0.0f, 0.0f, 1.0f });
    m_MinimapCamera->m_fCameraPitch = 0.f;
    m_MinimapCamera->CreatePerspectiveFov(L_PI * 0.1f, 1.0f, -1.0f, 10000.0f);
    //m_MinimapCamera->CreateOrthographic((float)256, (float)256, -1.0f, 10000.0f);

    m_MinimapPosCamera = std::make_shared<LCamera>();
    m_MinimapPosCamera->CreateLookAt({ 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f });
    m_MinimapPosCamera->m_fCameraPitch = 0.f;
    m_MinimapPosCamera->CreateOrthographic((float)256, (float)256, -1.0f, 10000.0f);
}

void InGameScene::PlayerInit()
{
    // PlayerSetting
    LGlobal::g_PlayerModel = new LPlayer;
    LGlobal::g_PlayerModel->m_pModel = LFbxMgr::GetInstance().GetPtr(L"army3.fbx");
    LGlobal::g_PlayerModel->CreateBoneBuffer();
    LGlobal::g_PlayerModel->FSM(FSMType::PLAYER);

    TMatrix matScale;
    TMatrix matRot;
    D3DXMatrixScaling(&matScale, 0.2f, 0.2f, 0.2f);
    LGlobal::g_PlayerModel->m_matControl *= matScale;
    D3DXMatrixRotationY(&matRot, 3.14159);
    LGlobal::g_PlayerModel->m_matControl *= matRot;

    m_ModelCamera->SetTarget(LGlobal::g_PlayerModel);

    std::wstring head = L"Head";
    std::wstring root = L"root";
  
    TMatrix Head = LGlobal::g_PlayerModel->m_pModel->m_NameMatrixMap[0][head];
    TMatrix Root = LGlobal::g_PlayerModel->m_pModel->m_NameMatrixMap[0][root];

    LGlobal::g_PlayerModel->SetOBBBox({ -30.0f, Root._42, -20.0f }, { 30.0f, Head._42, 30.0f }, 0.2f);
}

void InGameScene::CharacterInit()
{
    // Zombie
    LCharacterIO::GetInstance().CharacterRead(L"../../res/UserFile/Character/Zombie.bin", L"../../res/hlsl/CharacterShaderInAnimationData.hlsl");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Zombie_Attack_Anim.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Zombie_Death.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Zombie_TakeDamage.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Zombie_Walk_Lock.bin");
    LFbxMgr::GetInstance().m_ZombieMap.push_back(LFbxMgr::GetInstance().GetPtr(L"Zombie_Attack_Anim.fbx"));
    LFbxMgr::GetInstance().m_ZombieMap.push_back(LFbxMgr::GetInstance().GetPtr(L"Zombie_Death.fbx"));
    LFbxMgr::GetInstance().m_ZombieMap.push_back(LFbxMgr::GetInstance().GetPtr(L"Zombie_TakeDamage.fbx"));
    LFbxMgr::GetInstance().m_ZombieMap.push_back(LFbxMgr::GetInstance().GetPtr(L"Zombie_Walk_Lock.fbx"));

    // Tank
    LCharacterIO::GetInstance().CharacterRead(L"../../res/UserFile/Character/Tank.bin", L"../../res/hlsl/CharacterShader.hlsl");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Tank_Combo.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Tank_Death.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Tank_TakeDamage.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Tank_Walk.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Tank_Run.bin");

    // ZombieWaveSetting
    m_ZombieWave = std::make_shared<ZombieWave>();
    m_ZombieWave->m_CurrentWave = 0;
    NextWave();

    // Character
    LCharacterIO::GetInstance().CharacterRead(L"../../res/UserFile/Character/army3.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Fire_Rifle_Ironsights.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Reload_Rifle_Ironsights.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Sprint_Fwd_Rifle.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Walk_Fwd_Rifle_Ironsights.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Idle_Rifle_Ironsights.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Player_TakeDamage.bin");
    LAnimationIO::GetInstance().AnimationRead(L"../../res/UserFile/Animation/Player_Death.bin");

    // Item
    LCharacterIO::GetInstance().ItemRead(L"../../res/UserFile/Item/Assault_Rifle_A.bin");
    // form
    LExportIO::GetInstance().ExportRead(L"RightHandForm.bin");

    // PlayerSetting
    PlayerInit();

    // ItemSetting
    m_GunModel = std::make_shared<LModel>();
    m_GunModel->m_pModel = LFbxMgr::GetInstance().GetPtr(L"Assault_Rifle_A.fbx");

    m_GunModel->m_ParentBoneName = LExportIO::GetInstance().m_ItemParentName[0];
    m_GunModel->m_pModel->m_matScale = LExportIO::GetInstance().m_ItemScale[0];
    m_GunModel->m_pModel->m_matRotation = LExportIO::GetInstance().m_ItemRotation[0];
    m_GunModel->m_pModel->m_matTranslation = LExportIO::GetInstance().m_ItemTranslation[0];

    m_Select = new LSelect;
}

void InGameScene::NextWave()
{
    m_ZombieWave->m_CurrentWave++;

    if (m_ZombieWave->m_CurrentWave > 3)
    {
        IsEndGame = true;
        return;
    }

    int zombieCount = m_ZombieWave->m_WaveZombieCountList[m_ZombieWave->m_CurrentWave];
    m_ZombieWave->m_EnemyMap["Zombie"].resize(zombieCount);

    int tankCount = m_ZombieWave->m_WaveTankCountList[m_ZombieWave->m_CurrentWave];
    m_ZombieWave->m_EnemyMap["Tank"].resize(tankCount);

    m_ZombieWave->m_EnemyMap["LNPC"].resize(zombieCount + tankCount);
    
    for (int i = 0; i < zombieCount; i++)
    {
        m_ZombieWave->m_EnemyMap["Zombie"][i] = std::make_shared<Zombie>(LGlobal::g_PlayerModel);
        m_ZombieWave->m_EnemyMap["Zombie"][i]->m_pModel = LFbxMgr::GetInstance().GetPtr(L"Zombie.fbx");
        m_ZombieWave->m_EnemyMap["Zombie"][i]->CreateBoneBuffer();
        m_ZombieWave->m_EnemyMap["Zombie"][i]->FSM(FSMType::ENEMY);
        
        m_ZombieWave->m_EnemyMap["Zombie"][i]->m_matControl._41 = m_ZombieWave->GetRandomNumber();
        m_ZombieWave->m_EnemyMap["Zombie"][i]->m_matControl._43 = m_ZombieWave->GetRandomNumber();

        m_ZombieWave->m_EnemyMap["LNPC"][i] = m_ZombieWave->m_EnemyMap["Zombie"][i];
    }

    if (m_ZombieWave->IsFirstCreate)
    {
        m_ZombieWave->m_EnemyMap["Zombie"][0]->ComputeOffset();
    }
   
    for (int i = 0; i < tankCount; i++)
    {
        m_ZombieWave->m_EnemyMap["Tank"][i] = std::make_shared<Tank>(LGlobal::g_PlayerModel);
        m_ZombieWave->m_EnemyMap["Tank"][i]->m_pModel = LFbxMgr::GetInstance().GetPtr(L"Tank.fbx");
        m_ZombieWave->m_EnemyMap["Tank"][i]->CreateBoneBuffer();
        m_ZombieWave->m_EnemyMap["Tank"][i]->FSM(FSMType::TANK);

        m_ZombieWave->m_EnemyMap["Tank"][i]->m_matControl._41 = m_ZombieWave->GetRandomNumber();
        m_ZombieWave->m_EnemyMap["Tank"][i]->m_matControl._43 = m_ZombieWave->GetRandomNumber();

        m_ZombieWave->m_EnemyMap["LNPC"][zombieCount + i] = m_ZombieWave->m_EnemyMap["Tank"][i];
    }

    for (int i = 0; i < zombieCount; i++)
    {
        m_ZombieWave->m_EnemyMap["Zombie"][i]->m_Player = LGlobal::g_PlayerModel;
        m_ZombieWave->m_EnemyMap["Zombie"][i]->SetAnimationArrayTexture();
        m_ZombieWave->m_EnemyMap["Zombie"][i]->SetAnimationArraySRV();
        m_ZombieWave->m_EnemyMap["Zombie"][i]->CreateCurrentFrameBuffer();
    }

    // Collision
    std::wstring head = L"Head";
    std::wstring root = L"root";
    std::wstring shoulder = L"RightShoulder";
    std::wstring hand = L"RightHand";

    TMatrix Head = m_ZombieWave->m_EnemyMap["Zombie"][0]->m_pModel->m_NameMatrixMap[0][head];
    TMatrix Root = m_ZombieWave->m_EnemyMap["Zombie"][0]->m_pModel->m_NameMatrixMap[0][root];
    TMatrix RightShoulder = m_ZombieWave->m_EnemyMap["Zombie"][0]->m_pModel->m_NameMatrixMap[0][shoulder];
    TMatrix RightHand = m_ZombieWave->m_EnemyMap["Zombie"][0]->m_pModel->m_NameMatrixMap[0][hand];

    TMatrix TankHead = m_ZombieWave->m_EnemyMap["Zombie"][0]->m_pModel->m_NameMatrixMap[0][head];
    TMatrix TankRoot = m_ZombieWave->m_EnemyMap["Zombie"][0]->m_pModel->m_NameMatrixMap[0][root];
    TMatrix TankRightShoulder = m_ZombieWave->m_EnemyMap["Zombie"][0]->m_pModel->m_NameMatrixMap[0][shoulder];
    TMatrix TankRightHand = m_ZombieWave->m_EnemyMap["Zombie"][0]->m_pModel->m_NameMatrixMap[0][hand];

    for (int i = 0; i < zombieCount; i++)
    {
        m_ZombieWave->m_EnemyMap["Zombie"][i]->SetOBBBox({ -20.0f, Root._42, -5.0f }, { 20.0f, Head._42, 30.0f }, 0.2f);
        m_ZombieWave->m_EnemyMap["Zombie"][i]->SetOBBBoxRightHand({ RightHand._41, RightHand._42, -40.0f }, { RightShoulder._41, RightShoulder._42, 40.0f }, 0.2f);
    }

    for (int i = 0; i < tankCount; i++)
    {
        m_ZombieWave->m_EnemyMap["Tank"][i]->SetOBBBox({ -40.0f, TankRoot._42, -5.0f }, { 40.0f, TankHead._42 + 60.0f, 30.0f }, 0.2f);
        m_ZombieWave->m_EnemyMap["Tank"][i]->SetOBBBoxRightHand({ TankRightHand._41 - 40.0f, TankRightHand._42 - 40.0f, -40.0f }, { TankRightShoulder._41 + 40.0f, TankRightShoulder._42 + 40.0f, 40.0f }, 0.2f);
    }

    if (m_ZombieWave->m_CurrentWave > 3) return;

    if (!m_ZombieWave->IsFirstCreate)
    {
        UIManager::GetInstance().GetUIObject(L"EnemyCount")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(m_ZombieWave->m_EnemyMap["Zombie"].size());
        UIManager::GetInstance().GetUIObject(L"crr_Wave")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(m_ZombieWave->m_CurrentWave);
    }

    m_ZombieWave->IsFirstCreate = false;
}

void InGameScene::RenderObject()
{
    
    LGlobal::g_PlayerModel->m_pModel->m_DrawList[0]->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
    m_CBmatShadow.g_matShadow = LGlobal::g_PlayerModel->m_matControl * m_matViewLight * m_matProjLight * m_matTexture;
    D3DXMatrixTranspose(&m_CBmatShadow.g_matShadow, &m_CBmatShadow.g_matShadow);
    LGlobal::g_pImmediateContext->UpdateSubresource(m_pCBShadow.Get(), 0, NULL, &m_CBmatShadow, 0, 0);
    LGlobal::g_pImmediateContext->VSSetConstantBuffers(1, 1, m_pCBShadow.GetAddressOf());
    LGlobal::g_PlayerModel->m_pModel->m_DrawList[0]->PreRender();
    ID3D11ShaderResourceView* pSRV[] = { m_RT.m_pSRV.Get() };
    LGlobal::g_pImmediateContext->PSSetShaderResources(1, 1, pSRV);
    LGlobal::g_PlayerModel->Render();
  
}

void InGameScene::RenderShadow(TMatrix* matWorld, TMatrix* matShadow,
    TMatrix* matView, TMatrix* matProj)
{
    TMatrix matWorldShadow = (*matWorld) * (*matShadow);
    LGlobal::g_PlayerModel->m_pModel->m_DrawList[0]->SetMatrix( &matWorldShadow, matView, matProj);
    LGlobal::g_PlayerModel->Render();
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

ID3D11Buffer* InGameScene::CreateConstantBuffer(ID3D11Device* pd3dDevice, void* data, UINT iNumIndex, UINT iSize, bool bDynamic)
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


void InGameScene::InitializeObjects()
{
    SoundInit();
    CameraInit();
    CharacterInit();
    CreateShadowConstantBuffer();
}

void InGameScene::InitializeSkyBox()
{
    m_SkyBox = std::make_shared<LSkyBox>();
    m_SkyBox->Set();
    m_SkyBox->Create(L"../../res/hlsl/SkyBox.hlsl", L"../../res/sky/grassenvmap1024.dds");
}

void InGameScene::InitializePlayerIcon()
{
    m_playerIcon = std::make_shared<KObject>();
    m_playerIcon->Init();
    m_playerIcon->SetPos({ 0, 0, -1 });
    m_playerIcon->SetScale({ 10,10, 1 });
    m_playerIcon->Create(L"../../res/hlsl/CustomizeMap.hlsl", L"../../res/ui/PlayerIcon.png");
}

void InGameScene::InitializeBloodSplatters()
{
    m_bloodSplatter.resize(10);
    for (auto& blood : m_bloodSplatter)
    {
        blood = std::make_shared<KObject>();
        blood->Init();
        blood->Create(L"../../res/hlsl/CustomizeMap.hlsl", L"../../res/ui/Empty.png");
        blood->SetPos({ 0, 10000, 0 });
        blood->SetScale({ 10,10, 1 });
        blood->AddScripts(std::make_shared<Animator>(L"Anim.xml"));
        blood->SetIsRender(false);
    }
}


void InGameScene::InitializeTrees()
{
    m_TreeList.resize(30);
    auto treeObj = LFbxMgr::GetInstance().Load(L"../../res/fbx/tree/Tree.fbx", L"../../res/hlsl/LightShadowMap.hlsl");
    for (auto& tree : m_TreeList)
    {
        tree = std::make_shared<LModel>();
        tree->SetLFbxObj(treeObj);
        tree->CreateBoneBuffer();
        InitializeTreePosition(tree);
    }
}

void InGameScene::InitializeTreePosition(std::shared_ptr<LModel>& tree)
{
    DirectX::XMMATRIX rotationMatrix, scalingMatrix, worldMatrix, translationMatrix;
    float x = (rand() % 1800) - 900;
    float z = (rand() % 1800) - 900;
    translationMatrix = DirectX::XMMatrixTranslation(x, m_CustomMap->GetHeight(x, z), z);
    rotationMatrix = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(270.0f));
    scalingMatrix = DirectX::XMMatrixScaling(110.0f, 110.0f, 110.0f);
    worldMatrix = DirectX::XMMatrixIdentity();
    worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, rotationMatrix);
    worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, scalingMatrix);
    worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, translationMatrix);
    tree->m_matControl = worldMatrix;
}

void InGameScene::InitializeWalls()
{
    m_WallList.resize(40);
    wallObj = LFbxMgr::GetInstance().Load(L"../../res/fbx/wall/Concrete wall.fbx", L"../../res/hlsl/LightShadowMap.hlsl");
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            m_WallList[i * 10 + j] = std::make_shared<LModel>();
            m_WallList[i * 10 + j]->SetLFbxObj(wallObj);
            m_WallList[i * 10 + j]->CreateBoneBuffer();

            DirectX::XMMATRIX rotationMatrix, translationMatrix, worldMatrix, scalingMatrix;
            float rotationAngle;

            // 위치에 따라 회전 각도를 설정
            if (i == 0) // 북쪽 벽
            {
                rotationAngle = 90.0f;
                rotationMatrix = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(rotationAngle));
                translationMatrix = DirectX::XMMatrixTranslation(-1000.0f, 0.0f, -1000.0f + 200.0f * j);
            }
            else if (i == 1) // 동쪽 벽
            {
                rotationAngle = 180.0f;
                rotationMatrix = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(rotationAngle));
                translationMatrix = DirectX::XMMatrixTranslation(-1000.0f + 200.0f * j, 0.0f, 1000.0f);
            }
            else if (i == 2) // 남쪽 벽
            {
                rotationAngle = 270.0f;
                rotationMatrix = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(rotationAngle));
                translationMatrix = DirectX::XMMatrixTranslation(1000.0f, 0.0f, 1000.0f - 200.0f * j);
            }
            else // 서쪽 벽
            {
                rotationAngle = 0.0f;
                rotationMatrix = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(rotationAngle));
                translationMatrix = DirectX::XMMatrixTranslation(1000.0f - 200.0f * j, 0.0f, -1000.0f);
            }

            scalingMatrix = DirectX::XMMatrixScaling(1.0f, 2.0f, 1.0f);
            worldMatrix = DirectX::XMMatrixIdentity();
            worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, scalingMatrix);
            worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, rotationMatrix);
            worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, translationMatrix);

            m_WallList[i * 10 + j]->m_matControl = worldMatrix;
        }
    }
}

void InGameScene::InitializeWallPosition(std::shared_ptr<LModel>& wall, int i, int j)
{
    DirectX::XMMATRIX rotationMatrix, translationMatrix, worldMatrix, scalingMatrix;
    float rotationAngle;
    if (i == 0) rotationAngle = 90.0f;
    else if (i == 1) rotationAngle = 180.0f;
    else if (i == 2) rotationAngle = 270.0f;
    else rotationAngle = 0.0f;

    rotationMatrix = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(rotationAngle));
    translationMatrix = DirectX::XMMatrixTranslation(-1000.0f + 200.0f * j, 0.0f, 1000.0f - 200.0f * i);
    scalingMatrix = DirectX::XMMatrixScaling(1.0f, 2.0f, 1.0f);
    worldMatrix = DirectX::XMMatrixIdentity();
    worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, scalingMatrix);
    worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, rotationMatrix);
    worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, translationMatrix);
    wall->m_matControl = worldMatrix;
}

void InGameScene::InitializeBullets()
{
    auto bulletObj = LFbxMgr::GetInstance().Load(L"../../res/fbx/bullet/Tennis.fbx", L"../../res/hlsl/Bullet.hlsl");
    m_BulletList.resize(31);
    m_VisibleBulletList.resize(31);
    for (int i = 0; i < m_BulletList.size(); ++i)
    {
        m_VisibleBulletList[i] = false;
        m_BulletList[i] = std::make_shared<LModel>();
        m_BulletList[i]->SetLFbxObj(bulletObj);
        m_BulletList[i]->CreateBoneBuffer();
        InitializeBulletPosition(m_BulletList[i]);
    }
}

void InGameScene::InitializeBulletPosition(std::shared_ptr<LModel>& bullet)
{
    DirectX::XMMATRIX scalingMatrix = DirectX::XMMatrixScaling(0.1f, 0.1f, 0.1f);
    DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();
    worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, scalingMatrix);
    bullet->m_matControl = worldMatrix;
}

void InGameScene::InitializeLighting()
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

void InGameScene::InitializeShadow()
{
    m_pQuad.Set();
    m_pQuad.SetScreenVertex(0, 0, 250, 250, TVector2(LGlobal::g_WindowWidth, LGlobal::g_WindowHeight));
    m_pQuad.Create(L"../../res/hlsl/ShadowMap.hlsl", L"../../res/map/castle.jpg");

    m_RT.Create(1024, 1024);

    m_matTexture = TMatrix(0.5f, 0.0f, 0.0f, 0.0f
        , 0.0f, -0.5f, 0.0f, 0.0f
        , 0.0f, 0.0f, 1.0f, 0.0f
        , 0.5f, 0.5f, 0.0f, 1.0f);


}

void InGameScene::InitializeMap()
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

void InGameScene::InitializeGrasses()
{
    m_GrassList.resize(110);
    grassObj = LFbxMgr::GetInstance().Load(L"../../res/fbx/grass/Grass_green.fbx", L"../../res/hlsl/LightShadowMap.hlsl");
    for (auto& grass : m_GrassList)
    {
        grass = std::make_shared<LModel>();
        grass->SetLFbxObj(grassObj);
        grass->CreateBoneBuffer();
        {
            DirectX::XMMATRIX rotationMatrix, scalingMatrix, worldMatrix, translationMatrix;

            // make translation matrix randomly ( -1000 ~ 1000 )
            float x = (rand() % 1800) - 900;
            float z = (rand() % 1800) - 900;
            float y = m_CustomMap->GetHeight(x, z) + ((rand() % 5) + 5);

            translationMatrix = DirectX::XMMatrixTranslation(x, y, z);
            rotationMatrix = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(270.0f));
            scalingMatrix = DirectX::XMMatrixScaling(20.0f, 20.0f, 20.0f);
            worldMatrix = DirectX::XMMatrixIdentity();
            worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, rotationMatrix);
            worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, scalingMatrix);
            worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, translationMatrix);
            grass->m_matControl = worldMatrix;
        }
    }
}

void InGameScene::InitializeMinimap()
{
    m_ShapeMinimap.Set();
    m_ShapeMinimap.SetScreenVertex(10, 10, 256, 256, TVector2(LGlobal::g_WindowWidth, LGlobal::g_WindowHeight));
    m_ShapeMinimap.Create(L"../../res/hlsl/CustomizeMap.hlsl", L"../../res/ui/Hud_Box_128x64.png");
    m_rtMinimap.Create(256, 256);
}

void InGameScene::InitializeMuzzleFlash()
{
    //muzzleflash
    m_muzzleFlash = make_shared<KObject>();
    m_muzzleFlash->Init();
    m_muzzleFlash->Create(L"../../res/hlsl/CustomizeMap.hlsl", L"../../res/ui/muzzleflash.png");
    m_muzzleFlash->SetScale({ 10,10,1.f });
    m_muzzleFlash->SetPos({ 0,30,0 });
    // m_muzzleFlash->AddScripts(make_shared<BillBoard>());
}


///////////////////process/////////////////////

void InGameScene::ProcessMuzzleFlash()
{
    //muzzle Frame
    TMatrix matRotation, matTrans, matScale, worldMat;
    matScale = TMatrix::Identity;
    D3DXMatrixInverse(&matRotation, nullptr, &LGlobal::g_pMainCamera->m_matView);
    matRotation._41 = 0.0f;
    matRotation._42 = 0.0f;
    matRotation._43 = 0.0f;
    matRotation._44 = 1.0f;
    TVector3 foward;
    foward = LGlobal::g_PlayerModel->m_matControl.Forward();
    TVector3 vTrans = { m_GunModel->m_matControl._41 ,m_GunModel->m_matControl._42 ,m_GunModel->m_matControl._43 };
    vTrans = vTrans + (foward * 180);
    D3DXMatrixTranslation(&matTrans, vTrans.x,
        vTrans.y,
        vTrans.z
    );

    D3DXMatrixScaling(&matScale, m_muzzleFlash->m_vScale.x,
        m_muzzleFlash->m_vScale.y,
        m_muzzleFlash->m_vScale.z
    );
    worldMat = matScale * matRotation * matTrans;
    m_muzzleFlash->SetMatrix(&worldMat, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
    // m_muzzleFlash->SetPos({ m_GunModel->m_matControl._41,m_GunModel->m_matControl._42 ,m_GunModel->m_matControl._43 });
    // m_muzzleFlash->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
    m_muzzleFlash->Frame();
}

void InGameScene::ProcessBloodSplatter()
{
    for (auto obj : m_bloodSplatter)
    {
        if (obj->GetIsRender())
        {
            TMatrix matRotation, matTrans, matScale, worldMat;
            matScale = TMatrix::Identity;
            D3DXMatrixInverse(&matRotation, nullptr, &LGlobal::g_pMainCamera->m_matView);
            matRotation._41 = 0.0f;
            matRotation._42 = 0.0f;
            matRotation._43 = 0.0f;
            matRotation._44 = 1.0f;
            TVector3 foward;
            foward = LGlobal::g_PlayerModel->m_matControl.Forward();
            TVector3 vTrans = obj->m_vPosition;
            vTrans = vTrans + (foward * -180);
            D3DXMatrixTranslation(&matTrans, vTrans.x,
                vTrans.y,
                vTrans.z
            );

            D3DXMatrixScaling(&matScale, m_muzzleFlash->m_vScale.x,
                m_muzzleFlash->m_vScale.y,
                m_muzzleFlash->m_vScale.z
            );
            worldMat = matScale * matRotation * matTrans;

            obj->SetMatrix(&worldMat, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
            obj->Frame();
            if (obj->GetScript<Animator>(L"Animator")->_currentKeyframeIndex == 16)
            {
                obj->SetIsRender(false);
            }
        }
    }
}

void InGameScene::CheckPlayerDeath()
{
    if (LGlobal::g_PlayerModel->IsDeath)
    {
        IsEndGame = true;

    }
}

void InGameScene::PlayInGameSound()
{
    LGlobal::g_IngameSound->Play();
}

void InGameScene::UpdateUI()
{
    if (Init_2)
    {
        UIManager::GetInstance().GetUIObject(L"total_Wave")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(m_ZombieWave->m_WaveZombieCountList.size());
        UIManager::GetInstance().GetUIObject(L"crr_Wave")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(m_ZombieWave->m_CurrentWave);
        UIManager::GetInstance().GetUIObject(L"EnemyCount")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(m_ZombieWave->m_EnemyMap["Zombie"].size());
        Init_2 = false;
    }
}

void InGameScene::ProcessWaveTransition()
{
    if (IsNextWave && IsReleaseTank)
    {
        IsReleaseTank = false;
        IsNextWave = false;
        NextWave();
    }
}

void InGameScene::UpdateMapObjects()
{
    //m_MapModel->Frame();
    m_CustomMap->Frame();
}

void InGameScene::UpdateWallModels()
{
    for (int i = 0; i < m_WallList.size(); i++)
    {
        m_WallList[i]->Frame();
    }
}

void InGameScene::UpdateTreeModels()
{
    for (auto& tree : m_TreeList)
    {
        tree->Frame();
    }
}

void InGameScene::UpdateBulletModels()
{
    int index = LGlobal::g_BulletCount % m_BulletList.size();
    if (LInput::GetInstance().m_MouseState[0] > KEY_PUSH && LGlobal::g_BulletCount > 0 && LGlobal::g_PlayerModel->IsEndReload)
    {
        if (m_VisibleBulletList[index] == false)
        {
            m_VisibleBulletList[index] = true;
            TMatrix scale = TMatrix::CreateScale(0.03);
            m_BulletList[index]->m_matControl = scale * LGlobal::g_PlayerModel->m_matControl;

            m_BulletList[index]->m_matControl._42 += 33.f;
        }
    }
    for (int i = 0; i < m_BulletList.size(); i++)
    {
        if (m_VisibleBulletList[i])
        {
            m_BulletList[i]->Frame();

            m_BulletList[i]->m_matControl._41 += m_BulletList[i]->m_matControl.Forward().x * 5000;
            m_BulletList[i]->m_matControl._42 += m_BulletList[i]->m_matControl.Forward().y * 5000;
            m_BulletList[i]->m_matControl._43 += m_BulletList[i]->m_matControl.Forward().z * 5000;
            if (m_BulletList[i]->m_matControl._41 > 1000.f
                || m_BulletList[i]->m_matControl._41 < -1000.f
                || m_BulletList[i]->m_matControl._43 > 1000.f
                || m_BulletList[i]->m_matControl._43 < -1000.f
                || m_BulletList[i]->m_matControl._42 > 1000.f
                || m_BulletList[i]->m_matControl._42 < -1000.f)
            {
                m_VisibleBulletList[i] = false;
            }
        }
    }
}

void InGameScene::AdjustPlayerHeight()
{
    float fHeight = m_CustomMap->GetHeight(LGlobal::g_PlayerModel->m_matControl._41, LGlobal::g_PlayerModel->m_matControl._43);
    LGlobal::g_PlayerModel->m_matControl._42 = fHeight + 1.0f;
}

void InGameScene::SwitchCameraView()
{
    if (LInput::GetInstance().m_KeyStateOld[DIK_1] > KEY_PUSH)
    {
        LGlobal::g_pMainCamera = m_DebugCamera.get();
    }

    if (LInput::GetInstance().m_KeyStateOld[DIK_2] > KEY_PUSH)
    {
        LGlobal::g_pMainCamera = m_ModelCamera.get();
    }

    if (LInput::GetInstance().m_KeyStateOld[DIK_3] > KEY_PUSH)
    {
        LGlobal::g_pMainCamera = m_BackViewCamera.get();
    }
}

void InGameScene::UpdateCameraTargetPosition()
{
    m_ModelCamera->SetTargetPos(TVector3(LGlobal::g_PlayerModel->m_matControl._41 + 20, LGlobal::g_PlayerModel->m_matControl._42 + 15, LGlobal::g_PlayerModel->m_matControl._43));
}

void InGameScene::FramePlayerModel()
{
    LGlobal::g_PlayerModel->Frame();
    LGlobal::g_PlayerModel->Process();
}

void InGameScene::FrameGunModel()
{
    if (m_GunModel->m_pModel != nullptr && LGlobal::g_PlayerModel->m_pActionModel != nullptr)
    {
        if (LGlobal::g_PlayerModel->m_pActionModel->m_iEndFrame <= int(LGlobal::g_PlayerModel->m_fCurrentAnimTime)) return;

        m_GunModel->m_pModel->m_matSocket = LGlobal::g_PlayerModel->m_pActionModel->m_NameMatrixMap[int(LGlobal::g_PlayerModel->m_fCurrentAnimTime)][m_GunModel->m_ParentBoneName];

        m_GunModel->m_matControl = m_GunModel->m_pModel->m_matScale * m_GunModel->m_pModel->m_matRotation * m_GunModel->m_pModel->m_matSocket
            * m_GunModel->m_pModel->m_matTranslation * LGlobal::g_PlayerModel->m_matControl;
    }
}

void InGameScene::UpdateZombieAndTankModels()
{
    for (auto& zombie : m_ZombieWave->m_EnemyMap["Zombie"])
    {
        float fHeight = m_CustomMap->GetHeight(zombie->m_matControl._41, zombie->m_matControl._43);
        zombie->m_matControl._42 = fHeight + 1.0f;
    }

    for (auto& tank : m_ZombieWave->m_EnemyMap["Tank"])
    {
        float fHeight = m_CustomMap->GetHeight(tank->m_matControl._41, tank->m_matControl._43);
        tank->m_matControl._42 = fHeight + 1.0f;
    }

    //m_ZombieWave->CollisionCheckOBB(m_TreeList, m_ZombieWave->m_ZombieModelList);
    //m_ZombieWave->CollisionCheckOBB(m_TreeList, m_ZombieWave->m_TankList);

    m_ZombieWave->CollisionCheckByDistance(m_TreeList, m_ZombieWave->m_EnemyMap["Zombie"]);
    m_ZombieWave->CollisionCheckByDistance(m_TreeList, m_ZombieWave->m_EnemyMap["Tank"]);


    m_ZombieWave->Frame();
}

void InGameScene::HandlePlayerTreeCollisions()
{
    for (auto& tree : m_TreeList)
    {
        float offsetX = LGlobal::g_PlayerModel->m_OBBBox.m_Box.vCenter.x - tree->m_matControl._41;
        float offsetY = LGlobal::g_PlayerModel->m_OBBBox.m_Box.vCenter.y - tree->m_matControl._42;
        float offsetZ = LGlobal::g_PlayerModel->m_OBBBox.m_Box.vCenter.z - tree->m_matControl._43;

        TVector3 dir = { offsetX, offsetY, offsetZ };
        float distance = dir.Length();
        if (distance <= 33)
        {
            
            dir.Normalize();
            dir *= (33 - distance);
            LGlobal::g_PlayerModel->m_matControl._41 += dir.x;
            LGlobal::g_PlayerModel->m_matControl._43 += dir.z;
        }
    }

    for (auto& zombie : m_ZombieWave->m_EnemyMap["Zombie"])
    {
        float offsetX = LGlobal::g_PlayerModel->m_OBBBox.m_Box.vCenter.x - zombie->m_matControl._41;
        float offsetY = LGlobal::g_PlayerModel->m_OBBBox.m_Box.vCenter.y - zombie->m_matControl._42;
        float offsetZ = LGlobal::g_PlayerModel->m_OBBBox.m_Box.vCenter.z - zombie->m_matControl._43;

		TVector3 dir = { offsetX, offsetY, offsetZ };
		float distance = dir.Length();
        TVector3 range = zombie->m_OBBBox.m_Box.vMax - zombie->m_OBBBox.m_Box.vMin;
        float r = range.Length() * 0.7f;
        if (distance <= r)
        {
            dir.Normalize();
            dir *= (r - distance);
			LGlobal::g_PlayerModel->m_matControl._41 += dir.x;
			LGlobal::g_PlayerModel->m_matControl._43 += dir.z;
		}
	}

	for (auto& tank : m_ZombieWave->m_EnemyMap["Tank"])
	{
		float offsetX = LGlobal::g_PlayerModel->m_OBBBox.m_Box.vCenter.x - tank->m_matControl._41;
		float offsetY = LGlobal::g_PlayerModel->m_OBBBox.m_Box.vCenter.y - tank->m_matControl._42;
		float offsetZ = LGlobal::g_PlayerModel->m_OBBBox.m_Box.vCenter.z - tank->m_matControl._43;
		
        TVector3 dir = { offsetX, offsetY, offsetZ };
        float distance = dir.Length();
        TVector3 range = tank->m_OBBBox.m_Box.vMax - tank->m_OBBBox.m_Box.vMin;
        float r = range.Length() * 0.7f;
        if (distance <= r)
        {
			dir.Normalize();
			dir *= (r - distance);
			LGlobal::g_PlayerModel->m_matControl._41 += dir.x;
			LGlobal::g_PlayerModel->m_matControl._43 += dir.z;
		}
	}
}

void InGameScene::LimitPlayerMovement()
{
    if (LGlobal::g_PlayerModel->m_matControl._41 > 970.f) LGlobal::g_PlayerModel->m_matControl._41 = 970.f;
    if (LGlobal::g_PlayerModel->m_matControl._41 < -970.f) LGlobal::g_PlayerModel->m_matControl._41 = -970.f;
    if (LGlobal::g_PlayerModel->m_matControl._43 > 970.f) LGlobal::g_PlayerModel->m_matControl._43 = 970.f;
    if (LGlobal::g_PlayerModel->m_matControl._43 < -970.f) LGlobal::g_PlayerModel->m_matControl._43 = -970.f;
}

void InGameScene::UpdateGunModelPosition()
{
    if (m_GunModel->m_pModel != nullptr && LGlobal::g_PlayerModel->m_pActionModel != nullptr)
    {
        if (LGlobal::g_PlayerModel->m_pActionModel->m_iEndFrame <= int(LGlobal::g_PlayerModel->m_fCurrentAnimTime)) return;

        m_GunModel->m_pModel->m_matSocket = LGlobal::g_PlayerModel->m_pActionModel->m_NameMatrixMap[int(LGlobal::g_PlayerModel->m_fCurrentAnimTime)][m_GunModel->m_ParentBoneName];

        m_GunModel->m_matControl = m_GunModel->m_pModel->m_matScale * m_GunModel->m_pModel->m_matRotation * m_GunModel->m_pModel->m_matSocket
            * m_GunModel->m_pModel->m_matTranslation * LGlobal::g_PlayerModel->m_matControl;
    }
}

void InGameScene::FrameCollisionDetection()
{
    m_Select->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
    LGlobal::g_PlayerModel->m_OBBBox.UpdateOBBBoxPosition(
        { LGlobal::g_PlayerModel->m_OBBBox.m_matWorld._41,
            LGlobal::g_PlayerModel->m_OBBBox.m_matWorld._42,
            LGlobal::g_PlayerModel->m_OBBBox.m_matWorld._43 });


}

void InGameScene::FrameUI()
{
    UIManager::GetInstance().Frame();
}

void InGameScene::FramePointLight()
{
    // Frame point light
    m_PointLight[0].Frame(LGlobal::g_PlayerModel->m_matControl._41 + LGlobal::g_PlayerModel->m_matControl.Forward().x * 150,
        LGlobal::g_PlayerModel->m_matControl._42,
        LGlobal::g_PlayerModel->m_matControl._43 + LGlobal::g_PlayerModel->m_matControl.Forward().z * 150);
}

void InGameScene::InitializeOBBBox()
{

    LGlobal::g_PlayerModel->m_OBBBox.Frame();
    LGlobal::g_PlayerModel->m_OBBBox.CreateOBBBox(
        LGlobal::g_PlayerModel->m_SettingBox.fExtent[0],
        LGlobal::g_PlayerModel->m_SettingBox.fExtent[1],
        LGlobal::g_PlayerModel->m_SettingBox.fExtent[2],
        { LGlobal::g_PlayerModel->m_OBBBox.m_matWorld._41,
            LGlobal::g_PlayerModel->m_OBBBox.m_matWorld._42,
            LGlobal::g_PlayerModel->m_OBBBox.m_matWorld._43 },
        LGlobal::g_PlayerModel->m_SettingBox.vAxis[0],
        LGlobal::g_PlayerModel->m_SettingBox.vAxis[1],
        LGlobal::g_PlayerModel->m_SettingBox.vAxis[2]);
    m_BackViewCamera->SetTarget(LGlobal::g_PlayerModel);
}