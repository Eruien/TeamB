#pragma once
#include "MonoBehaviour.h"
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
	void CreateConstantBuffer();
	void Init() override;
	void Frame() override;
	void UpdateHp(float hp = 50);
private :
	ComPtr<ID3D11Buffer> _cbBuffer;
};

