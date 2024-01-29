#pragma once
#include "LSpriteObj.h"
#include "LInput.h"
#include "LWrite.h"
#include "LDebugCamera.h"

class LParticle
{
public:
	bool     m_bLife = true;
	TVector3 m_vPos;
	TVector3 m_vScale;
	TVector3 m_vRotation;

public:

	float   m_fElapsedTimer = 0.0f;
	float	m_fOffsetTime = 0.0f;
	float	m_fAnimTimer = 1.0f;
	float	m_fPlayTime = 0.0f;
	int		m_iCurrentAnimIndex = 0;
	std::basic_string<wchar_t>   m_szName;
	int		m_iID = 0;
	int		m_iMaxFrame = 0;

	bool	m_bBilboard = false;
	bool	m_bFadeout = false;
	bool	m_bScreenEffect = false;


	void Frame()
	{
		if (!m_bLife) return;
		m_fPlayTime += LGlobal::g_fSPF;
		if (m_iMaxFrame <= 1)
		{
			
			if (m_fPlayTime > m_fAnimTimer)
			{
				m_iCurrentAnimIndex = 0;
				m_bLife = false;
				m_fPlayTime=0.0f;
			}
		}
		else
		{
			
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
	}
};

class LEffect	:LSpriteObj
{
public:
	static LEffect& GetInstance()
	{
		static LEffect input;
		return input;
	}

public:
	std::unique_ptr<LSpriteTexture> m_pSpriteTexObj = nullptr;
	std::unique_ptr<LSpriteUV> m_pSpriteUVObj = nullptr;
	std::unique_ptr<LSpriteObj> m_pSpriteAirObj = nullptr;
	std::unique_ptr<LSpriteTexture> m_pScreenTexObj = nullptr;
public:

	std::map<int, LSpriteObj*>	m_SpriteList;
	std::list<LParticle>		m_ParticleList;
	int	m_iParticleIndex = 0;
	int m_iSpriteIndex = 0;
	int m_iTexIndex = 0;


public:


	void SetBilboardMatrix()
	{
		//빌보드 축 선택기능추가 필요
		//SetBuilboardXMatrix();
		//	SetBuilboardYMatrix();
		//	SetBuilboardZMatrix();
		// 
		TMatrix mat = m_matView;
		D3DXMatrixInverse(&mat, NULL, &mat);
		//mat._41 = 0.0f;
		//mat._42 = 0.0f;
		//mat._43 = 0.0f;
		//mat._44 = 1.0f;
		m_matWorld = m_matWorld* mat;

	}

    LFrustum m_Frustum;
public:
    TVector3 m_vLook;
    TVector3 m_vUp;
    TVector3 m_vRight;
    float m_fSpeed = 10.0f;
public:
    TMatrix m_matView;
    TMatrix m_matOrthoProjection;
    TMatrix m_matProj;
    TVector3 m_vCameraPos;
    TVector3 m_vTargetPos;
    DWORD m_dwWindowWidth;
    DWORD m_dwWindowHeight;
public:
    TMatrix   CreateLookAt(TVector3 vPosition, TVector3 vTarget, TVector3 vUp = { 0,1,0 });
    TMatrix   CreatePerspectiveFov(float fFov, float fAspect, float fNear, float fFar);
    void	UpdateVector();
public:
    virtual bool Init();
    virtual bool Frame();
    virtual bool Render();
    virtual bool Release();
public:
    void Create(TVector3 pos, TVector2 size);
public:
    virtual ~LEffect() {}
};

