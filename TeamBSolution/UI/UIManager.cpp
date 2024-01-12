#include "UIManager.h"
#include "LStd.h"
#include "tinyxml2.h"
#include "IntToSprite.h"
#include "DigitDisplay.h"
#include "PickingUI.h"
#include "DragUI.h"
#include "Resize2D.h"
#include "imgui_menuBar.h"
#include "ImGuiManager.h"
#include "ImguiDetail.h"
#include "Animator.h"
#include "ChangeTexture.h"
#include "ExitWindow.h"
void UIManager::Init()
{
	


}

void UIManager::Frame()
{
	for (auto obj : _objs)
	{
		obj->Frame();
	}
}

void UIManager::Render()
{
	for (auto obj : _objs)
	{
		obj->Render();
	}
}

void UIManager::Save()
{
	tinyxml2::XMLDocument doc;

	tinyxml2::XMLElement* root = doc.NewElement("UIScene");
	doc.LinkEndChild(root);

	for (auto obj : _objs)
	{
		//Name
		string nameStr(obj->GetName().begin(), obj->GetName().end());
		root->SetAttribute("Name", nameStr.c_str()); 
		//Pos
		tinyxml2::XMLElement* node = doc.NewElement("Position");
		root->LinkEndChild(node);
		node->SetAttribute("x", obj->m_vPosition.x);
		node->SetAttribute("y", obj->m_vPosition.y);
		node->SetAttribute("z", obj->m_vPosition.z);
		//Scale
		tinyxml2::XMLElement* node2 = doc.NewElement("Scale");
		root->LinkEndChild(node2);
		node2->SetAttribute("width", obj->m_vScale.x);
		node2->SetAttribute("height", obj->m_vScale.y);
		//texture
		tinyxml2::XMLElement* node3 = doc.NewElement("Texture");
		root->LinkEndChild(node3);
		node3->SetAttribute("texPath", wtm(obj->m_Tex->m_texPath).c_str());
		//scripts
		tinyxml2::XMLElement* node4 = doc.NewElement("Scripts");
		root->LinkEndChild(node4);
		for (auto script : obj->GetScripts())
		{
			node4->SetAttribute("script", wtm(script->GetName()).c_str());
			if (script->GetName() == L"Animator")
			{
				tinyxml2::XMLElement* node5 = doc.NewElement("args");
				root->LinkEndChild(node5);
				node5->SetAttribute("animationPath", wtm(obj->GetScript<Animator>(L"Animator")->GetCurrentAnimation()->GetPath()).c_str());
			}
			if (script->GetName() == L"DigitDisplay")
			{
				tinyxml2::XMLElement* node6 = doc.NewElement("args");
				root->LinkEndChild(node6);
			//	node6->SetAttribute("TexPaths", );
			}
		}
		
	}
}

void UIManager::Load()
{
}

shared_ptr<KObject> UIManager::GetUIObject(wstring name)
{
	for (auto obj : _objs)
	{
		if (obj->GetName() == name)
			return obj;
	}
	return nullptr;
}


