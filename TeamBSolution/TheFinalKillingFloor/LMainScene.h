#pragma once
#include "LScene.h"

class LMainScene : public SceneState
{
public:
	bool Init() override;
	void Frame();
	void Render();
	void Process() override;
	void Release();
public:
	LMainScene(LScene* parent);
	virtual ~LMainScene();
};

