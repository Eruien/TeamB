#pragma once
#include "LCore.h"
#include "LMap.h"

class Sample : public LCore
{
public:
	std::shared_ptr<LDebugCamera> m_DebugCamera = nullptr;
	std::unique_ptr<LPlaneObj> m_pMapObj = nullptr;//����
	ID3D11BlendState* m_AlphaBlend = nullptr;//�ӽ�
public:

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	virtual ~Sample();
};

