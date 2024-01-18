#pragma once
#include "LCore.h"
#include "LScene.h"

class Sample : public LCore
{
public:
	LScene* m_Scene = nullptr;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	virtual ~Sample();
};

