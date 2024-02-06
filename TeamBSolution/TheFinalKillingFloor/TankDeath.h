#pragma once
#include "Tank.h"

class TankDeath : public TankState
{
public:
	bool Init() override;
	void Process() override;
	void Release();
public:
	TankDeath(Tank* parent);
	virtual ~TankDeath();
};

