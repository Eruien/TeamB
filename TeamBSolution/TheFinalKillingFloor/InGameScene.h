#pragma once
#include "LScene.h"

class InGameScene : public SceneState
{
public:
	bool Init() override;
	void Process() override;
	void Release();
public:
	InGameScene(LScene* parent);
	virtual ~InGameScene();
};

