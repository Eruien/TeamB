#pragma once
#include "LScene.h"
#include "LMap.h"
#include "LPlayer.h"
#include "LFbxMgr.h"

class InGameScene : public SceneState
{
public:
	//std::shared_ptr<LMap> m_CustomMap = nullptr;
	shared_ptr<LModel> m_Map = nullptr;
	std::shared_ptr<LModel> m_PlayerModel = nullptr;
	LFbxObj* fbxObj = nullptr;

	// fbx map
	LFbxObj* fbxMap = nullptr;
public:
	bool Init() override;
	void Render() override;
	void Process() override;
	void Release();
public:
	InGameScene(LScene* parent);
	virtual ~InGameScene();
};

