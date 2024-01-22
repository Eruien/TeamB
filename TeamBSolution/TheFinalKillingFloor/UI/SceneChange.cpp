#include "SceneChange.h"
#include "../LScene.h"
#include "PickingUI.h"
#include "KObject.h"
#include "UIManager.h"
SceneChange::SceneChange(Event event) : MonoBehaviour(L"SceneChange")
{
	_sceneEvent = event;
}

SceneChange::SceneChange() : MonoBehaviour(L"SceneChange")
{
}

SceneChange::~SceneChange()
{
}

void SceneChange::Init()
{

}

void SceneChange::Frame()
{

	PICKING_STATE state = GetGameObject()->GetScript<PickingUI>(L"PickingUI")->GetButtonState();

	if (state == PICKING_STATE::UP)
	{
		if (_sceneEvent == Event::GOMAINSCENE)
		{
			UIManager::GetInstance().Load(L"MainScene.xml");
			LScene::GetInstance().SetTransition(_sceneEvent);
		}
		else if (_sceneEvent == Event::GOINGAMESCENE)
		{
			UIManager::GetInstance().Load(L"IngameScene.xml");
			LScene::GetInstance().SetTransition(_sceneEvent);
		}
		else if (_sceneEvent == Event::GOENDSCENE)
		{
			UIManager::GetInstance().Load(L"EndScene.xml");
			LScene::GetInstance().SetTransition(_sceneEvent);
		}


	}
}


