#include "InGameScene.h"
#include "LGlobal.h"
#include "LInput.h"
#include "LWrite.h"

bool InGameScene::Init()
{
    fbxObj = LFbxMgr::GetInstance().Load(L"../../res/fbx/army/army3.fbx", L"../../res/hlsl/CharacterInstancing.hlsl");
    LFbxMgr::GetInstance().Load(L"../../res/fbx/army/Animation/Fire_Rifle_7.fbx", L"../../res/hlsl/CustomizeMap.hlsl");
    LFbxMgr::GetInstance().Load(L"../../res/fbx/army/Animation/Reload_Rifle_65.fbx", L"../../res/hlsl/CustomizeMap.hlsl");
    LFbxMgr::GetInstance().Load(L"../../res/fbx/army/Animation/Run_Rifle_44.fbx", L"../../res/hlsl/CustomizeMap.hlsl");
    LFbxMgr::GetInstance().Load(L"../../res/fbx/army/Animation/Walk_Rifle_55.fbx", L"../../res/hlsl/CustomizeMap.hlsl");
    LFbxMgr::GetInstance().Load(L"../../res/fbx/army/Animation/Idle_Rifle_189.fbx", L"../../res/hlsl/CustomizeMap.hlsl");

    m_PlayerModel = std::make_shared<LPlayer>();
    m_PlayerModel->SetLFbxObj(fbxObj);
    m_PlayerModel->CreateBoneBuffer();
    m_PlayerModel->FSM(FSMType::PLAYER);
    return true;
}

void InGameScene::Frame()
{
    m_PlayerModel->Frame();
    m_PlayerModel->Process();
}

void InGameScene::Render()
{
    m_PlayerModel->Render();
    
    std::wstring textState = L"InGameScene";
    LWrite::GetInstance().AddText(textState, 320.0f, 500.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
}

void InGameScene::Process()
{
    Frame();
    Render();

    if (LInput::GetInstance().m_KeyState[DIK_ESCAPE] == DWORD(KEY_PUSH))
    {
        Release();
        m_pOwner->SetTransition(Event::GOENDSCENE);
        return;
    }

    if (LInput::GetInstance().m_KeyState[DIK_ESCAPE] == DWORD(KEY_PUSH))
    {
        Release();
        m_pOwner->SetTransition(Event::GOMAINSCENE);
        return;
    }
}

void InGameScene::Release()
{

}

InGameScene::InGameScene(LScene* parent) : SceneState(parent)
{
    Init();
}
InGameScene::~InGameScene() {}