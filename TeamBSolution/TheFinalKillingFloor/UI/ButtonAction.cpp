#include "ButtonAction.h"
#include "PickingUI.h"
#include "KObject.h"
#include "TextureList.h"
#include "LWrite.h"
#include "LGlobal.h"
#include "../LScene.h"
ButtonAction::ButtonAction(wstring texPaths) : MonoBehaviour(L"ButtonAction")
{
	_texXmlPath = texPaths;
	LoadTextureList(_texXmlPath);
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
