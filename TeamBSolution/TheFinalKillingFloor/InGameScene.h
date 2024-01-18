#pragma once
#include "LScene.h"
#include "LMap.h"

class InGameScene : public SceneState
{
public:
	std::shared_ptr<LMap> m_CustomMap = nullptr;
	std::shared_ptr<LPlayer> m_PlayerModel = nullptr;
public:
	bool Init() override;
	void Process() override;
	void Release();
public:
	InGameScene(LScene* parent);
	virtual ~InGameScene();
};

