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
public:
	EnemyTakeDamage(LNPC* parent);
	virtual ~EnemyTakeDamage();
};

