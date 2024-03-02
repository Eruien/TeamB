#pragma once
#include "LNPC.h"

class EnemyTakeDamage : public NPCState
{
public:
	bool Init() override;
	void Process() override;
	void Release();
	void UpdateHPbar();
	bool m_Timer = false;
	bool IsTakeDamageAnimation = false;
	float m_AnimationHP = 50.0f;
	float m_MinusHP = 50.0f;
public:
	EnemyTakeDamage(LNPC* parent);
	virtual ~EnemyTakeDamage();
};

