#pragma once
#include "MonoBehaviour.h"

struct hpVertex
{
	TVector3		p;
	TVector3		n;
	TVector4		c;
	TVector2		t;
	float				hp;
};

struct cbData_hp
{
	float hp;
	float dump;
	float dump1;
	float dump2;
};
class HpBar : public MonoBehaviour
{
public:
	HpBar();
	~HpBar();
public:
	void Init() override;
	void Frame() override;
	void UpdateHp(float hp = 50);
	void SetCrrHp(float hp) { _testCrrHp = hp; }
private :
	float _maxHp = 100;
	float _testCrrHp;
};

