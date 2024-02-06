#pragma once
#include "Tank.h"

class TankTakeDamage : public TankState
{
public:
	bool Init() override;
	void Process() override;
	void Release();
	void UpdateHPbar();
	bool m_Timer = false;
public:
	TankTakeDamage(Tank* parent);
	virtual ~TankTakeDamage();
};

