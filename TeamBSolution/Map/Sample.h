#pragma once
#include "LCore.h"
#include "LFbxMgr.h"
#include "LModel.h"
#include "LSelect.h"

class Sample : public LCore
{
public:
	std::shared_ptr<LDebugCamera> m_DebugCamera = nullptr;
	std::shared_ptr<LModel> ZombieModel = nullptr;
	LObject* m_Triangle = nullptr;
	LFbxObj* fbxObj = nullptr;
	LSelect* m_Select = nullptr;


public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	virtual ~Sample();
};

