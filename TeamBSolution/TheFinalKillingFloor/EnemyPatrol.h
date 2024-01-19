#pragma once
#include "LNPC.h"

class EnemyPatrol : public NPCState
{
public:
	bool Init() override;
	void Process() override;
	void Release();
public:
	EnemyPatrol(LNPC* parent);
	virtual ~EnemyPatrol();
};

