#pragma once
#include "LCore.h"
#include "LScene.h"
#include "LModelCamera.h"
#include "LMap.h"
#include "LFbxMgr.h"

class Sample : public LCore
{
public:
	std::shared_ptr<LDebugCamera> m_DebugCamera = nullptr;
	std::shared_ptr<LModelCamera> m_ModelCamera = nullptr;
	LScene* m_Scene = nullptr;

	std::shared_ptr<LMap> m_CustomMap = nullptr;
	std::shared_ptr<LModel> m_PlayerModel = nullptr;
	LFbxObj* fbxObj = nullptr;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	virtual ~Sample();
};

