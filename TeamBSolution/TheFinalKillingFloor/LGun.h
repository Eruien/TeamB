#pragma once
#include "LModel.h"

class LGun : public LModel
{
public:
	float m_ShotDelay = 0.1f;
	float m_StartShoot = 0.0f;
	float m_DPS = 0.0f;
};

