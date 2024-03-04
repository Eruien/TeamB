#pragma once
#include "LModel.h"

class LGun
{
public:
	float m_ShotDelay = 0.1f;
	std::map<LModel, GunState> WeaponList;
};

