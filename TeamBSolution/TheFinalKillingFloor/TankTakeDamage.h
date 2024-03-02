#pragma once
#include "Tank.h"

class TankTakeDamage : public NPCState
{
public:
	bool Init() override;
	void Process() override;
	void Release();
	void UpdateHPbar();
	bool m_Timer = false;
	bool IsTakeDamageAnimation = false;
	float m_AnimationHP = 70.0f;
	float m_MinusHP = 30.0f;
public:
	TankTakeDamage(Tank* parent);
	virtual ~TankTakeDamage();
};

