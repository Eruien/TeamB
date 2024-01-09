#pragma once
#include "MonoBehaviour.h"
class IntToSprite : public MonoBehaviour
{
public:
	IntToSprite(int num, vector<wstring> tex);
	~IntToSprite();
	void Init() override;
	void Frame() override;

private:
	int _num;
	vector<wstring> _numTex;
};

