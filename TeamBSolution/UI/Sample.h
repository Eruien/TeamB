#pragma once
#include "LCore.h"
#include "KObject.h"
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
	shared_ptr<KObject> obj;
	vector<shared_ptr<KObject>> _objects;
	shared_ptr< KObject> sObj;
	shared_ptr< KObject> sObj2;
	shared_ptr<imgui_test> _imgui;
	shared_ptr<ImGuiManager> _imGuiManager;
	static shared_ptr<KObject> _clickedObject;
public:
	virtual ~Sample();
};

