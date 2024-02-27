#include "ButtonAction.h"
#include "PickingUI.h"
#include "KObject.h"
#include "TextureList.h"
#include "LWrite.h"
#include "LGlobal.h"
#include "../LScene.h"
#include "UIManager.h"
ButtonAction::ButtonAction(wstring texPaths, wstring bFuntion) : MonoBehaviour(L"ButtonAction")
{
	_texXmlPath = texPaths;
	LoadTextureList(_texXmlPath);
	_function = bFuntion;


	_functionMap[L"MainSceneOption"] = &ButtonAction::MainSceneOption;
	_functionMap[L"MainSceneOptionClose"] = &ButtonAction::MainSceneOptionClose;
	_functionMap[L"RetryButton"] = &ButtonAction::RetryButton;
	_functionMap[L"SoundToggle"] = &ButtonAction::SoundToggle;
}
ButtonAction::~ButtonAction()
{
}
void ButtonAction::Frame()
{
	//-(LINPUT.GetMousePos().y - LGlobal::g_WindowHeight / 2)
	PICKING_STATE state = GetGameObject()->GetScript<PickingUI>(L"PickingUI")->GetButtonState();


	if (state == PICKING_STATE::NONE)
	{
		GetGameObject()->m_Tex = LManager<LTexture>::GetInstance().Load(_texList->GetTexList()[0]);
		GetGameObject()->m_Tex->Apply();
	}
	else if (state == PICKING_STATE::HOVER)
	{
		GetGameObject()->m_Tex = LManager<LTexture>::GetInstance().Load(_texList->GetTexList()[1]);
		GetGameObject()->m_Tex->Apply();
	}
	else if (state == PICKING_STATE::HOLD)
	{
		GetGameObject()->m_Tex = LManager<LTexture>::GetInstance().Load(_texList->GetTexList()[2]);
		GetGameObject()->m_Tex->Apply();
	}
	else if (state == PICKING_STATE::UP)
	{
		GetGameObject()->m_Tex = LManager<LTexture>::GetInstance().Load(_texList->GetTexList()[0]);
		GetGameObject()->m_Tex->Apply();

		if (_functionMap.find(_function) != _functionMap.end())
		{
			(this->*_functionMap[_function])();
		}
		else {
			std::cout << "Function not found" << std::endl;
		}

	}
	//if (state == PICKING_STATE::NONE)
	//{
	//	GetGameObject()->m_Tex = LManager<LTexture>::GetInstance().Load(L"../../res/ui/button1.png");
	//	GetGameObject()->m_Tex->Apply();
	//}
	//else if (state == PICKING_STATE::HOVER)
	//{
	//	GetGameObject()->m_Tex = LManager<LTexture>::GetInstance().Load(L"../../res/ui/button2.png");
	//	GetGameObject()->m_Tex->Apply();
	//}
	//else if (state == PICKING_STATE::HOLD)
	//{
	//	GetGameObject()->m_Tex = LManager<LTexture>::GetInstance().Load(L"../../res/ui/button3.png");
	//	GetGameObject()->m_Tex->Apply();
	//}
}
void ButtonAction::LoadTextureList(wstring texXmlPath)
{
	_texList = make_shared<TextureList>();
	_texList->Load(texXmlPath);
}

void ButtonAction::MainSceneOption()
{

	vector<shared_ptr<KObject>> group;
	group = UIManager::GetInstance().GetGroup(L"MainOptionMenu");
	for (auto obj : group)
	{
		obj->SetIsRender(true);
	}
}

void ButtonAction::MainSceneOptionClose()
{
	vector<shared_ptr<KObject>> group;
	group = UIManager::GetInstance().GetGroup(L"MainOptionMenu");
	for (auto obj : group)
	{
		obj->SetIsRender(false);
	}
}

void ButtonAction::RetryButton()
{
	
	UIManager::GetInstance().Load(L"MainScene.xml");
	
}

void ButtonAction::SoundToggle()
{
	if (_toggleSound == false)
		UIManager::GetInstance().GetUIObject(L"SoundToggleText")->GetScript<Text>(L"Text")->SetText(L"Sound Off");
	else
	{
		UIManager::GetInstance().GetUIObject(L"SoundToggleText")->GetScript<Text>(L"Text")->SetText(L"Sound On");
	}
	_toggleSound = !_toggleSound;
	LGlobal::g_IngameSound->ToggleSound(_toggleSound);
	LGlobal::g_BackgroundSound->ToggleSound(_toggleSound);
	LGlobal::g_EffectSound1->ToggleSound(_toggleSound);
	LGlobal::g_EffectSound2->ToggleSound(_toggleSound);
	

	
}
