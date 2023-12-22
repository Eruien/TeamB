#pragma once
#include "LPlaneObj.h"
struct LUVRect
{
	bool empty()
	{
		if (m_Max.x == 0 && m_Max.y == 0)
		{
			return true;
		}
		return false;
	}

	TVector2 m_Min;
	TVector2 m_Max;

};

struct LSpriteInfo
{
	int			iNumRow;
	int			iNumColumn;
	TVector3	p;
	TVector3	s;
	float		fAnimTimer;
	float		fElapsedTimer; // 에니메이션 가동 시간
	std::basic_string<wchar_t>		texFile;
	std::basic_string<wchar_t>		texAlphaFile;
	std::basic_string<wchar_t>		shaderFile;
	std::vector<std::basic_string<TCHAR>> texList;
	void Reset()
	{
		iNumRow = 1;
		iNumColumn = 1;
		fAnimTimer = 1.0f;
		fElapsedTimer = 0.0f;
		p = { 1.0f, 1.0f, 1.0f };
		s = { 0.0f, 0.0f, 0.0f };
		texList.clear();
		texFile = L"";
		texAlphaFile = L"";
		shaderFile = L"";
	}
	LSpriteInfo()
	{
		Reset();
	}


};
class LSpriteObj : public LPlaneObj
{
public:
	static LSpriteObj& GetInstance()
	{
		static LSpriteObj input;
		return input;
	}
public:

	/*const*/	LTexture* m_pAlphaTex = nullptr;
	float	m_fAnimTimer = 1.0f;  // 전체 시간
	float	m_fOffsetTime = 0.0f;	// 1프레임 교체시간
	float	m_fElapsedTimer = 0.0f; // 누적시간
	int		m_iCurrentAnimIndex = 0;
	int		m_iNumSpriteX = 1;
	int		m_iNumSpriteY = 1;
	LSpriteInfo m_InitSpriteInfo;
public:
	virtual bool   Init() override;

	virtual bool   Render() override;
	virtual int    GetMaxSize() { return 1; }
	virtual bool  Load(	LSpriteInfo) ;
	virtual bool  LoadTexArray(std::vector<std::basic_string<TCHAR>>& texList) { return true; };
	virtual void  SetUVFrame(int iNumRow, int iNumColumn) {}


};

class LSpriteTexture : public LSpriteObj
{
public:
	static LSpriteTexture& GetInstance()
	{
		static LSpriteTexture input;
		return input;
	}

public:
	std::vector</*const*/ LTexture*>  m_pTexList;
public:
	virtual int    GetMaxSize() { return m_pTexList.size(); }
	virtual bool   LoadTexArray(std::vector<std::basic_string<TCHAR>>& texList) override;
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
public:
	LSpriteTexture();
	virtual ~LSpriteTexture();
};
class LSpriteUV : public LSpriteObj
{
public:
	static LSpriteUV& GetInstance()
	{
		static LSpriteUV input;
		return input;
	}

public:
	std::vector<LUVRect>  m_pUVList;
	void  SetUVFrame(int iNumRow, int iNumColumn) override;
	void	SetPixelVertex(LUVRect uv);
	void  SetNumSprite(int x, int y)
	{
		m_iNumSpriteX = 1;
		m_iNumSpriteY = 1;
	}
public:
	virtual int    GetMaxSize() { return m_pUVList.size(); }
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
public:
	LSpriteUV();
	virtual ~LSpriteUV();
};
