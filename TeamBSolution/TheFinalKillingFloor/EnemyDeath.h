#pragma once
#include "LNPC.h"

class EnemyDeath : public NPCState
{
public:
	bool Init() override;
	void Process() override;
	void Release();
public:
	EnemyDeath(LNPC* parent);
	virtual ~EnemyDeath();
};

