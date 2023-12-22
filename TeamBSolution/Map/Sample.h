#pragma once
#include "LCore.h"
#include "LHeightMap.h"
#include "LQurdTree.h"
#include "LPick.h"

class Sample : public LCore
{
public:
	std::shared_ptr<LDebugCamera> m_DebugCamera = nullptr;
	LHeightMap* m_HeightMap = nullptr;
	LQurdtree* m_Tree = nullptr;
	LPick& _pick = LPick::GetInstance();
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	Sample() {};
	virtual ~Sample();
};

