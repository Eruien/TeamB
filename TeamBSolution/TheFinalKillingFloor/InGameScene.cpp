#include "InGameScene.h"
#include "LGlobal.h"
#include "LInput.h"
#include "LWrite.h"

bool InGameScene::Init()
{
    return true;
}

void InGameScene::Frame()
{

}

void InGameScene::Render()
{
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

}
InGameScene::~InGameScene() {}