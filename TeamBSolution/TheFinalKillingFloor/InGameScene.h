#pragma once
#include "LScene.h"
#include "LMap.h"
#include "LPlayer.h"
#include "LFbxMgr.h"
#include "LDebugCamera.h"
#include "LModelCamera.h"

class InGameScene : public SceneState
{
public:
	std::shared_ptr<LDebugCamera> m_DebugCamera = nullptr;
	std::shared_ptr<LModelCamera> m_ModelCamera = nullptr;
public:
	std::shared_ptr<LMap> m_CustomMap = nullptr;
	std::shared_ptr<LPlayer> m_PlayerModel = nullptr;
	std::shared_ptr<LModel> m_MapModel = nullptr;
	LFbxObj* fbxObj = nullptr;
	LFbxObj* mapObj = nullptr;
public:
	bool Init() override;
	void Render() override;
	void Process() override;
	void Release();
public:
	InGameScene(LScene* parent);
	virtual ~InGameScene();
};

