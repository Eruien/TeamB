#pragma once
#include "LNPC.h"

class EnemyTakeDamage : public NPCState
{
public:
	bool Init() override;
	void Process() override;
	void Release();
public:
	EnemyTakeDamage(LNPC* parent);
	virtual ~EnemyTakeDamage();
};

