#pragma once
#include "LObject.h"
#include "LFSMMgr.h"
#include "LAnimation.h"

class LPlaneObj : public LObject
{
public:
	bool m_DeleteFlag = false;
	bool m_IsDamaged = false;
	bool isInvincibility = false;
public:
	virtual bool CreateVertexBuffer() override;
public:
	virtual bool Init() override;
	virtual bool Frame() override;
public:
	virtual ~LPlaneObj() {}
};


