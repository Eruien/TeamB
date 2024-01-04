#pragma once
#include "LCore.h"
#include "KSpriteObj.h"
#include "UICamera.h"
#include "ImGuiManager.h"
class imgui_test;
class LPlaneObj;
class Sample : public LCore
{
public:
	std::shared_ptr<UICamera> m_DebugCamera = nullptr;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
	shared_ptr<LPlaneObj> obj;
	vector<shared_ptr<KSpriteObj>> _objects;
	shared_ptr< KSpriteObj> sObj;
	shared_ptr< KSpriteObj> sObj2;
	shared_ptr<imgui_test> _imgui;
	shared_ptr<ImGuiManager> _imGuiManager;
	static shared_ptr<KSpriteObj> _clickedObject;
public:
	virtual ~Sample();
};

