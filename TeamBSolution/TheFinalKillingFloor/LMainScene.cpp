#include "LMainScene.h"
#include "LGlobal.h"
#include "LInput.h"
#include "LWrite.h"
#include "LInput.h"
#include "UI/UIManager.h"
bool LMainScene::Init()
{
	for (auto obj : UIManager::GetInstance().GetSceneObj(L"MainScene.xml"))
	{
		obj->SetIsRender(true);
		auto group = UIManager::GetInstance().GetGroup(L"MainOptionMenu");
		for (auto obj : group)
		{
			obj->SetIsRender(false);
		}
	}
	for (auto obj : UIManager::GetInstance().GetSceneObj(L"IngameScene.xml"))
	{
		obj->SetIsRender(false);
	}
	for (auto obj : UIManager::GetInstance().GetSceneObj(L"EndScene.xml"))
	{
		obj->SetIsRender(false);
	}
	for (auto obj : UIManager::GetInstance().GetSceneObj(L"Shop1.xml"))
	{
		obj->SetIsRender(false);
	}
    LSoundMgr::GetInstance().Load(L"../../res/sound/BackgroundSound.mp3");
	LSoundMgr::GetInstance().GetPtr(L"BackgroundSound.mp3")->Play();
	return true;
    
}

void LMainScene::Render()
{
   /* std::wstring textState = L"Main Scene";
    LWrite::GetInstance().AddText(textState, 320.0f, 500.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
    std::wstring debugText = L"Key State: " + std::to_wstring(LInput::GetInstance().m_KeyStateOld[DIK_1]);
    LWrite::GetInstance().AddText(debugText, 320.0f, 400.0f, { 1.0f, 1.0f, 1.0f, 1.0f });*/
    UIManager::GetInstance().Render();
}

void LMainScene::Process()
{
    //m_pOwner->SetTransition(Event::GOINGAMESCENE);
    UIManager::GetInstance().Frame();
}

void LMainScene::Release()
{
 
}

LMainScene::LMainScene(LScene* parent) : SceneState(parent)
{
    Init();
}
LMainScene::~LMainScene() {}