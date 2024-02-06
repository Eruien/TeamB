#pragma once
#include "LNPC.h"

class Tank;

class TankState
{
public:
	Tank* m_pOwner = nullptr;
public:
	virtual bool Init() { return true; }
	virtual	void Process() = 0;
public:
	TankState(Tank* parent) : m_pOwner(parent) {}
	virtual ~TankState() {}
};

class Tank : public LNPC
{
};

