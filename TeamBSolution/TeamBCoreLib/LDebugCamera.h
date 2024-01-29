#pragma once
#include "LCamera.h"

class LDebugCamera : public LCamera
{
public:
	float m_fCameraYaw = 0.0f;
	float m_fCameraPitch = 0.0f;
	float m_fCameraRoll = 0.0f;
public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
};

