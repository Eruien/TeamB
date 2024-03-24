#pragma once
#include "Boss.h"

class BossTakeDamage : public NPCState
{
public:
	bool Init() override;
	void Process() override;
	void Release();
public:
	BossTakeDamage(Boss* parent);
	virtual ~BossTakeDamage();
};

