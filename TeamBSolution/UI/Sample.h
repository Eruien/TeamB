#pragma once
#include "LCore.h"
#include "KObject.h"
#include "UICamera.h"
#include "ImGuiManager.h"
#include "Imgui_ObjectDetail.h"
#include "imgui_menuBar.h"
#include "NumberObject.h"
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
	shared_ptr<KObject> sObj;
	shared_ptr<KObject> sObj2;
	shared_ptr<NumberObject> nObj;
	shared_ptr< Imgui_ObjectDetail> _imGuiObjDetail;
	shared_ptr< imgui_menuBar> _imgui_menuBar;
	shared_ptr<ImGuiManager> _imGuiManager;
	static shared_ptr<KObject> s_selectedObject;
	static bool s_isMouseInImGuiWindow;
public:
	virtual ~Sample();
};

