#pragma once
#include "LCore.h"
#include "LHeightMap.h"
#include "LQurdTree.h"
#include "LInput.h"

class Sample : public LCore
{
public:
	std::shared_ptr<LDebugCamera> m_DebugCamera = nullptr;
	LHeightMap* m_HeightMap = nullptr;
	LQurdtree* m_Tree = nullptr;

	//for picking. ½ÃÁø.
	LInput m_Input = LInput::GetInstance();
public:
	TVector3 m_PickedPoint;
	bool ProcessPicking();
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	virtual ~Sample();
};

