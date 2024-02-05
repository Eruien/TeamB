#pragma once
#include "MonoBehaviour.h"

struct cb_DataA
{
	float alpha[4];
};
class UIEvent : public MonoBehaviour
{
public:
	UIEvent(wstring Function);
	~UIEvent();
public:
	void Frame() override;
public :
	void HitPlayerEffect();
	void CreateConstantBuffer();
private:
	float _crrTime = 0;
	float _duration = 1.0f;
	cb_DataA data;
	ComPtr<ID3D11Buffer> _cBuff;
	std::unordered_map<wstring, void (UIEvent::*)()> _functionMap;
	wstring _function = L"None";

};

