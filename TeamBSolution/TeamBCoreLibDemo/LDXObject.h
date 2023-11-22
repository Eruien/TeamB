#pragma once
#include "LStd.h"
#include "LManager.h"
#include "LManagerAgr.h"

struct SimpleVertex
{
	LVector p;
	LVector n;
	LVector4 c;
	LVector2 t;
};

struct CB_Data
{
	LMatrix matWorld;
	LMatrix matView;
	LMatrix matProj;
};

class LDXObject
{
public:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pImmediateContext = nullptr;
	ID3D11Buffer* m_pVertexBuffer = nullptr;
	ID3D11Buffer* m_pIndexBuffer = nullptr;
	ID3D11Buffer* m_pConstantBuffer = nullptr;
	ID3D11InputLayout* m_pVertexLayout = nullptr;
	LTexture* m_Tex = nullptr;
	LShader* m_Shader = nullptr;
	CB_Data m_cbData;
	std::vector<SimpleVertex> m_VertexList;
	std::vector<DWORD> m_IndexList;
public:
	void Set(ID3D11Device* pDevice, ID3D11DeviceContext* pImmediateContext);
	virtual bool CreateVertexData();
	virtual bool CreateIndexData();
	virtual bool CreateVertexBuffer();
	virtual bool CreateIndexBuffer();
	virtual bool CreateConstantBuffer();
	virtual bool CreateLayout();
	virtual bool Create(std::wstring ShaderFileName, std::wstring texFileName);
public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool PreRender();
	virtual bool Render();
	virtual bool PostRender();
	virtual bool Release();
public:
	virtual ~LDXObject() {}
};

