#pragma once
#include "Boss.h"

class BossSwiping : public NPCState
{
public:
	bool Init() override;
	void Process() override;
	void Release();
public:
	BossSwiping(Boss* parent);
	virtual ~BossSwiping();
};

