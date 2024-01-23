#pragma once
#include "LCore.h"
#include "LMap.h"

class LParticle
{
public:
	bool     m_bLife = true;
	TVector3 m_vPos;
	TVector3 m_vSclae;
	TVector3 m_vRotation;
	float   m_fElapsedTimer = 0.0f;
	float	m_fOffsetTime = 0.0f;
	float	m_fAnimTimer = 1.0f;
	float	m_fPlayTime = 0.0f;
	int		m_iCurrentAnimIndex = 0;
	std::basic_string<wchar_t>   m_szName;
	int		m_iID=0;
	int		m_iMaxFrame=0;
	void Frame()
	{
		if (m_iMaxFrame <= 1 || !m_bLife) return;
		m_fPlayTime += LGlobal::g_fSPF;
		m_fElapsedTimer += LGlobal::g_fSPF;
		if (m_fElapsedTimer >= m_fOffsetTime)
		{
			m_iCurrentAnimIndex++;
			if (m_iCurrentAnimIndex >= m_iMaxFrame)
			{
				m_iCurrentAnimIndex = 0;
				m_bLife = false;
			}
			m_fElapsedTimer -= m_fOffsetTime;
			
		}
	}
};
class Sample : public LCore
{
public:
	std::shared_ptr<LDebugCamera> m_DebugCamera = nullptr;
	std::unique_ptr<LPlaneObj> m_pMapObj = nullptr;//수정
	ID3D11BlendState* m_AlphaBlend = nullptr;//임시
	std::unique_ptr<LSpriteTexture> m_pSpriteTexObj = nullptr;
	std::unique_ptr<LSpriteUV> m_pSpriteUVObj = nullptr;
	std::unique_ptr<LSpriteObj> m_pSpriteAirObj = nullptr;
public:

	std::map<int, LSpriteObj*>	m_SpriteList;
	std::list<LParticle>		m_ParticleList;
	int	m_iParticleIndex = 0;
	
	int   m_iSpriteIndex = 0;

	int m_iTexIndex = 0;

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	virtual ~Sample();
};

