#include "LSpriteObj.h"
bool  LSpriteObj::Load(LSpriteInfo info)
{
	m_InitSpriteInfo = info;
	m_fAnimTimer = info.fAnimTimer;
	m_iNumSpriteX = info.iNumRow;
	m_iNumSpriteY = info.iNumColumn;
	Set();
	SetPos(info.p);
	SetScale(info.s);
	if (!info.texAlphaFile.empty())
	{
		
		this->m_pAlphaTex = LManager<LTexture>::GetInstance().Load(info.texAlphaFile);
	}
	LoadTexArray(info.texList);
	SetUVFrame(info.iNumRow, info.iNumColumn);
	return Create(info.shaderFile, info.texFile);
}
bool LSpriteObj::Init()
{
	static LSpriteInfo ExampleInfo;

	ExampleInfo.iNumRow = 1;
	ExampleInfo.iNumColumn = 3;
	ExampleInfo.fAnimTimer = 1.0f;
	ExampleInfo.fElapsedTimer = 0.0f;
	ExampleInfo.p = { 1.0f, 1.0f, 1.0f };
	ExampleInfo.s = { 1.0f, 1.0f, 1.0f };
	ExampleInfo.texList.clear();
	ExampleInfo.texFile = L"../../res/effect/star3.png";
	//ExampleInfo.texAlphaFile = L"";
	//ExampleInfo.shaderFile = L"../../res/hlsl/PVShader.hlsl";
	ExampleInfo.shaderFile = L"../../res/hlsl/HeightMap.hlsl";
	//ExampleInfo.shaderFile = L"../../res/hlsl/CustomizeMap.hlsl";

	Load(ExampleInfo);
	return true;

}
bool LSpriteObj::Render()
{
	PreRender();
	if (m_pAlphaTex)
	{
		m_pAlphaTex->Apply();
	}
	PostRender();
	return true;
}
bool   LSpriteTexture::LoadTexArray(std::vector<std::basic_string<TCHAR>>& texList)
{
	for (auto& texname : texList)
	{
		/*const*/ LTexture* pTex = LManager<LTexture>::GetInstance().Load(texname);//???
		m_pTexList.push_back(pTex);
	}
	m_fOffsetTime = m_fAnimTimer / m_pTexList.size();
	return true;
}
bool LSpriteTexture::Init()
{
	LPlaneObj::Init();


	LTexture* m_pAlphaTex = nullptr;
	float	m_fAnimTimer = 1.0f;  // 전체 시간
	float	m_fOffsetTime = 0.0f;	// 1프레임 교체시간
	float	m_fElapsedTimer = 0.0f; // 누적시간
	int		m_iCurrentAnimIndex = 0;
	int		m_iNumSpriteX = 1;
	int		m_iNumSpriteY = 1;
	
	return true;
}
bool LSpriteTexture::Frame()
{
	LPlaneObj::Frame();
	

	m_fElapsedTimer += LGlobal::g_fSPF;
	if (m_pTexList[m_iCurrentAnimIndex] != nullptr)
	{
		if (m_fElapsedTimer >= m_fOffsetTime)
		{
			m_iCurrentAnimIndex++;
			if (m_iCurrentAnimIndex >= m_pTexList.size())
			{
				m_iCurrentAnimIndex = 0;
			}
			m_fElapsedTimer -= m_fOffsetTime;
		}
	}
	return true;
}
bool LSpriteTexture::Render()
{
	PreRender();
	m_pTexList[m_iCurrentAnimIndex]->Apply();
	PostRender();
	return true;
}
bool LSpriteTexture::Release()
{
	LPlaneObj::Release();
	return true;
}

LSpriteTexture::LSpriteTexture()
{
	Init();
}
LSpriteTexture::~LSpriteTexture()
{
	Release();
}

void LSpriteUV::SetUVFrame(int iNumRow, int iNumColumn)
{
	LUVRect tRt;
	TVector2 uv;

	float fOffsetX = 1.0f / iNumColumn;
	float fOffsetY = 1.0f / iNumRow;
	for (int row = 0; row < iNumRow; row++)
	{
		uv.y = fOffsetY * row;
		for (int column = 0; column < iNumColumn; column++)
		{
			uv.x = fOffsetX * column;
			tRt.m_Min = uv;
			tRt.m_Max.x = uv.x + fOffsetX;
			tRt.m_Max.y = uv.y + fOffsetY;
			m_pUVList.push_back(tRt);
		}
	}
	m_fOffsetTime = m_fAnimTimer / m_pUVList.size();
}
bool LSpriteUV::Init()
{
	LPlaneObj::Init();

	return true;
}
bool LSpriteUV::Frame()
{
	LPlaneObj::Frame();
	m_fElapsedTimer += LGlobal::g_fSPF;
	if (m_fElapsedTimer >= m_fOffsetTime)
	{
		m_iCurrentAnimIndex++;
		if (m_iCurrentAnimIndex >= m_pUVList.size())
		{
			m_iCurrentAnimIndex = 0;
		}
		m_fElapsedTimer -= m_fOffsetTime;
	}
	return true;
}
void LSpriteUV::SetPixelVertex(LUVRect uv)
{

	m_VertexList[0].t = uv.m_Min;
	m_VertexList[1].t.x = uv.m_Max.x;
	m_VertexList[1].t.y = uv.m_Min.y;

	m_VertexList[2].t.x = uv.m_Min.x;
	m_VertexList[2].t.y = uv.m_Max.y;
	m_VertexList[3].t = m_VertexList[2].t;

	m_VertexList[4].t = m_VertexList[1].t;
	m_VertexList[5].t = uv.m_Max;

	m_pImmediateContext->UpdateSubresource(
		m_pVertexBuffer.Get(), 0, nullptr, &m_VertexList.at(0), 0, 0);//????

}
bool LSpriteUV::Render()
{
	SetPixelVertex(m_pUVList[m_iCurrentAnimIndex]);
	PreRender();
	PostRender();
	return true;
}
bool LSpriteUV::Release()
{
	LPlaneObj::Release();
	return true;
}

LSpriteUV::LSpriteUV()
{
	Init();
}
LSpriteUV::~LSpriteUV()
{
	Release();
}