#pragma once

#pragma once
#include "LScene.h"
class SelectScene :public SceneState
{


public:
	bool Init() override;
	void Render() override;
	void Process() override;
	void Release();
public:
	SelectScene(LScene* parent);
	virtual ~SelectScene();
};


