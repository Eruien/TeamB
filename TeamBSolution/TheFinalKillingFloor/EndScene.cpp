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

    if (LInput::GetInstance().m_KeyStateOld[DIK_ESCAPE] == KEY_PUSH)
    {
        Release();
        LScene::GetInstance().SetTransition(Event::GOMAINSCENE);
        return;
    }

    if (LInput::GetInstance().m_KeyStateOld[DIK_ESCAPE] == KEY_PUSH)
    {
        Release();
        LScene::GetInstance().SetTransition(Event::GOINGAMESCENE);
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