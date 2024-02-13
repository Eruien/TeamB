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
	void Init() override;
	void Frame() override;
	void Render() override;
public :
	void HitPlayerEffect();
	void CreateConstantBuffer();
	wstring _function = L"None";
private:
	bool _isHited = false;
	float _crrTime = 0;
	float _duration = 1.0f;
	cb_DataA data;
	ComPtr<ID3D11Buffer> _cBuff;
	std::unordered_map<wstring, void (UIEvent::*)()> _functionMap;
	

};
