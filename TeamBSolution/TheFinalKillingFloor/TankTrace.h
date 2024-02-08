#pragma once
#include "Tank.h"

class TankTrace : public TankState
{
public:
	bool Init() override;
	void Process() override;
	void Release();
public:
	TankTrace(Tank* parent);
	virtual ~TankTrace();
};

