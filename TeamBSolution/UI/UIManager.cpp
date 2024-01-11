#include "UIManager.h"
#include "LStd.h"
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


