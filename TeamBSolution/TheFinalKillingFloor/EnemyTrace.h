#pragma once
#include "LNPC.h"

class EnemyTrace : public NPCState
{
public:
	bool Init() override;
	void Process() override;
	void Release();
public:
	EnemyTrace(LNPC* parent);
	virtual ~EnemyTrace();
};

