#pragma once
#include "MonoBehaviour.h"
class HpBar : public MonoBehaviour
{
public:
	HpBar();
	~HpBar();
public:
	void Init() override;
	void Frame() override;

};

