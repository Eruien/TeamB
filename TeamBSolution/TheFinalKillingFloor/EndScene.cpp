#include "EndScene.h"
#include "LGlobal.h"
#include "LInput.h"
#include "LWrite.h"

bool EndScene::Init()
{
    return true;
}

void EndScene::Render()
{
    
}

void EndScene::Process()
{
    std::wstring textState = L"EndScene";
    LWrite::GetInstance().AddText(textState, 320.0f, 500.0f, { 1.0f, 1.0f, 1.0f, 1.0f });

    if (LInput::GetInstance().m_KeyState[DIK_ESCAPE] == DWORD(KEY_PUSH))
    {
        Release();
        m_pOwner->SetTransition(Event::GOMAINSCENE);
        return;
    }

    if (LInput::GetInstance().m_KeyState[DIK_ESCAPE] == DWORD(KEY_PUSH))
    {
        Release();
        m_pOwner->SetTransition(Event::GOINGAMESCENE);
        return;
    }
}

void EndScene::Release()
{

}

EndScene::EndScene(LScene* parent) : SceneState(parent)
{

}
EndScene::~EndScene() {}