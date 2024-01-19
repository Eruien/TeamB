#include "LMainScene.h"
#include "LGlobal.h"
#include "LInput.h"
#include "LWrite.h"
#include "LInput.h"

bool LMainScene::Init()
{
	return true;
}

void LMainScene::Render()
{
    std::wstring textState = L"Main Scene";
    LWrite::GetInstance().AddText(textState, 320.0f, 500.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
    std::wstring debugText = L"Key State: " + std::to_wstring(LInput::GetInstance().m_KeyStateOld[DIK_1]);
    LWrite::GetInstance().AddText(debugText, 320.0f, 400.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
}

void LMainScene::Process()
{
    m_pOwner->SetTransition(Event::GOINGAMESCENE);
    if (g_InputData.bLeftClick)
    {
        Release();
        m_pOwner->SetTransition(Event::GOINGAMESCENE);
        return;
    }
}

void LMainScene::Release()
{
 
}

LMainScene::LMainScene(LScene* parent) : SceneState(parent)
{
  
}
LMainScene::~LMainScene() {}