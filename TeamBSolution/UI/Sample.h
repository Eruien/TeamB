#pragma once
#include "LCore.h"
#include "KSpriteObj.h"
#include "UICamera.h"

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
	shared_ptr< KSpriteObj> sObj;
	shared_ptr<imgui_test> _imgui;
public:
	virtual ~Sample();
};

