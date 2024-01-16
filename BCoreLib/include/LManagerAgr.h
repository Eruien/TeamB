#pragma once
#include "LStd.h"

class LTexture
{
public:
	static LTexture& GetInstance()
	{
		static LTexture input;
		return input;
	}
public:
	ID3D11ShaderResourceView* m_pTexSRV = nullptr;
	wstring m_texPath;
public:
	void Apply();
	bool Load(std::wstring fileName);
public:
	bool Release();
public:
	LTexture();
	virtual ~LTexture();
};

class LShader
{
public:
	static LShader& GetInstance()
	{
		static LShader input;
		return input;
	}
private:
	ID3D11VertexShader* m_pVS = nullptr;
	ID3D11PixelShader* m_pPS = nullptr;
public:
	ID3DBlob* m_pVSBlob = nullptr;
	ID3DBlob* m_pPSBlob = nullptr;
public:
	void Apply();
	bool Load(std::wstring fileName);
	bool LoadVertexShader(std::wstring fileName);
	bool LoadPixelShader(std::wstring fileName);
public:
	bool Release();
public:
	LShader();
	virtual ~LShader();
};

class LManagerAgr
{
};

