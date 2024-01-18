#include "InGameScene.h"
#include "LGlobal.h"
#include "LInput.h"
#include "LWrite.h"

bool InGameScene::Init()
{
    fbxObj = LFbxMgr::GetInstance().Load(L"../../res/fbx/army/army3.fbx", L"../../res/hlsl/ObjectInstancing.hlsl");
    /*LFbxMgr::GetInstance().Load(L"../../res/fbx/army/Animation/Fire_Rifle_7.fbx", L"../../res/hlsl/CustomizeMap.hlsl");
    LFbxMgr::GetInstance().Load(L"../../res/fbx/army/Animation/Reload_Rifle_65.fbx", L"../../res/hlsl/CustomizeMap.hlsl");
    LFbxMgr::GetInstance().Load(L"../../res/fbx/army/Animation/Run_Rifle_44.fbx", L"../../res/hlsl/CustomizeMap.hlsl");
    LFbxMgr::GetInstance().Load(L"../../res/fbx/army/Animation/Walk_Rifle_55.fbx", L"../../res/hlsl/CustomizeMap.hlsl");
    LFbxMgr::GetInstance().Load(L"../../res/fbx/army/Animation/Idle_Rifle_189.fbx", L"../../res/hlsl/CustomizeMap.hlsl");*/

    m_PlayerModel = std::make_shared<LModel>();
    m_PlayerModel->SetLFbxObj(fbxObj);
    m_PlayerModel->CreateBoneBuffer();
    //m_PlayerModel->FSM(FSMType::PLAYER);

    //m_CustomMap = std::make_shared<LMap>();
    //m_CustomMap->Set();
    //LMapDesc CMapDesc = {};
    //CMapDesc.iNumCols = 513;
    //CMapDesc.iNumRows = 513;
    //CMapDesc.fCellDistance = 1.0f;
    //CMapDesc.ShaderFilePath = L"../../res/hlsl/ShadowMap.hlsl";
    //CMapDesc.TextureFilePath = L"../../res/map/topdownmap.jpg";
    //m_CustomMap->Load(CMapDesc);

    fbxMap = LFbxMgr::GetInstance().Load(L"../../res/map/Mountain.fbx", L"../../res/hlsl/CustomizeMap.hlsl");
    m_Map = make_shared<LModel>();
    m_Map->SetLFbxObj(fbxMap);
    m_Map->CreateBoneBuffer();
    return true;
}

void InGameScene::Render()
{
    //m_CustomMap->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
    //m_CustomMap->Render();
    fbxMap->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
    m_Map->Render();
    m_PlayerModel->Render();
    
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
}

void InGameScene::Process()
{
    fbxMap->Frame();
    //m_CustomMap->Frame();
    m_PlayerModel->Frame();
}

void InGameScene::Release()
{

}

InGameScene::InGameScene(LScene* parent) : SceneState(parent)
{
    Init();
}
InGameScene::~InGameScene() {}