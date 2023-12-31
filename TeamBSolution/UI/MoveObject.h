#pragma once
#include "MonoBehaviour.h" 
class MoveObject : public MonoBehaviour
{
public:
	MoveObject();
	virtual ~MoveObject();
public:
	virtual void Init() override;
	virtual void Frame() override;

};

