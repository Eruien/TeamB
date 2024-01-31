#pragma once
#include "LModel.h"

class LGun : public LModel
{
public:
	float m_ShotDelay = 0.1f;
	int m_AmmunitionCount = 30;
};

