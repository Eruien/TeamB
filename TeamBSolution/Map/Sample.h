#pragma once
#include "LCore.h"
#include "LHeightMap.h"

class Sample : public LCore
{
public:
	shared_ptr<LDebugCamera> m_DebugCamera = nullptr;
	shared_ptr<HeightMap> map;
public:
	bool Init();
	bool Frame();
	bool PostFrame();
	bool PreRender();
	bool Render();
	bool PostRender();
	bool Release();
public:
	virtual ~Sample();
};

