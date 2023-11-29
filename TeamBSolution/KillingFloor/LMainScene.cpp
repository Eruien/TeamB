#include "LMainScene.h"
#include "LGlobal.h"
#include "LInput.h"
#include "LWrite.h"

bool LMainScene::Init()
{
	return true;
}

void LMainScene::Process()
{
    std::wstring textState = L"Main Scene";
    LWrite::GetInstance().AddText(textState, 320.0f, 500.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
    
    if (LInput::GetInstance().m_dwKeyState['1'] == DWORD(KeyState::KEY_PUSH))
    {
        Release();
        m_pOwner->SetTransition(Event::GOUISCENE);
        return;
    }

    if (LInput::GetInstance().m_dwKeyState['2'] == DWORD(KeyState::KEY_PUSH))
    {
        Release();
        m_pOwner->SetTransition(Event::GOMAPSCENE);
        return;
    }

    if (LInput::GetInstance().m_dwKeyState['3'] == DWORD(KeyState::KEY_PUSH))
    {
        Release();
        m_pOwner->SetTransition(Event::GOCHARACTERSCENE);
        return;
    }

    if (LInput::GetInstance().m_dwKeyState['4'] == DWORD(KeyState::KEY_PUSH))
    {
        Release();
        m_pOwner->SetTransition(Event::GOEFFECTSCENE);
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