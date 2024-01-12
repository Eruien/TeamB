#pragma once
#include "MonoBehaviour.h"
class DigitDisplay : public MonoBehaviour
{
public:
	DigitDisplay(int digit, wstring texPaths);
	~DigitDisplay();

	void Init() override;
	void Frame() override;
	void Render() override;
	void UpdateNumber();
	void Load();
	void Save();
private:
	vector<shared_ptr<KObject>> _objects;
	wstring _texXmlPath;
	vector<wstring> _texList;
	int _digitNum;

	int _num = 923; // 
};

