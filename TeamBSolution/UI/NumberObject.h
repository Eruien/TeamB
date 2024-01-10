#pragma once
#include "KObject.h"
class NumberObject : public KObject
{
public:
	NumberObject(int digitNum) : _digitNum(digitNum) {};
	virtual ~NumberObject() {};

	bool Init() override;
	bool Frame() override;
	bool Render()override;
	void UpdateNumber();

private:
	vector<shared_ptr<KObject>> _objects;
	int _digitNum;
	
	int _num = 923; // 
};

