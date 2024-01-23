#pragma once
#include "LCore.h"
#include "LMap.h"

class Sample : public LCore
{
public:
	std::shared_ptr<LDebugCamera> m_DebugCamera = nullptr;
	std::unique_ptr<LPlaneObj> m_pMapObj = nullptr;//수정
	ID3D11BlendState* m_AlphaBlend = nullptr;//임시
public:

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	virtual ~Sample();
};

