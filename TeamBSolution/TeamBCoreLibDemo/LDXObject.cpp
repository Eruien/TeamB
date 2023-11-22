#include "LDXObject.h"

void LDXObject::Set(ID3D11Device* pDevice, ID3D11DeviceContext* pImmediateContext)
{
	m_pDevice = pDevice;
	m_pDevice->AddRef();
	m_pImmediateContext = pImmediateContext;
	m_pImmediateContext->AddRef();
}

bool LDXObject::CreateVertexData()
{
	return true;
}

bool LDXObject::CreateIndexData()
{
	return true;
}

bool LDXObject::CreateVertexBuffer()
{
	D3D11_BUFFER_DESC bufferDesc = { 0, };
	bufferDesc.ByteWidth = sizeof(SimpleVertex) * m_VertexList.size();
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA initialData = { 0, };
	initialData.pSysMem = &m_VertexList.at(0);

	HRESULT hr = m_pDevice->CreateBuffer(
		&bufferDesc,
		&initialData,
		&m_pVertexBuffer);

	if (FAILED(hr))
	{
		MessageBoxA(NULL, "Create VertexBuffer Error", "Error Box", MB_OK);
		return false;
	}

	return true;
}

bool LDXObject::CreateIndexBuffer()
{
	if (m_IndexList.size() == 0) return true;

	D3D11_BUFFER_DESC bufferDesc = { 0, };
	bufferDesc.ByteWidth = sizeof(DWORD) * m_IndexList.size();
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA initialData = { 0, };
	initialData.pSysMem = &m_IndexList.at(0);

	HRESULT hr = m_pDevice->CreateBuffer(
		&bufferDesc,
		&initialData,
		&m_pIndexBuffer);

	if (FAILED(hr))
	{
		MessageBoxA(NULL, "Create IndexBuffer Error", "Error Box", MB_OK);
		return false;
	}

	return true;
}

bool LDXObject::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC Desc{};

	Desc.ByteWidth = sizeof(CB_Data);
	Desc.Usage = D3D11_USAGE_DEFAULT;
	Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	HRESULT hr = m_pDevice->CreateBuffer(
		&Desc,
		nullptr,
		&m_pConstantBuffer);

	if (FAILED(hr))
	{
		MessageBoxA(NULL, "Create ConstantBuffer Error", "Error Box", MB_OK);
		return false;
	}

	return true;
}

bool LDXObject::CreateLayout()
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	HRESULT hr = m_pDevice->CreateInputLayout(
		layout, sizeof(layout) / sizeof(layout[0]),
		m_Shader->m_pVSBlob->GetBufferPointer(),
		m_Shader->m_pVSBlob->GetBufferSize(),
		&m_pVertexLayout);

	if (FAILED(hr))
	{
		MessageBoxA(NULL, "Create Input Layout Error", "Error Box", MB_OK);
		return false;
	}

	return true;
}

bool LDXObject::Create(std::wstring ShaderFileName, std::wstring texFileName)
{
	CreateVertexData();
	CreateIndexData();
	CreateVertexBuffer();
	CreateIndexBuffer();
	CreateConstantBuffer();
	// obj공용 // m_pVS, m_pPS 쉐이더 생성
	m_Shader = LManager<LShader>::GetInstance().Load(ShaderFileName);
	CreateLayout();
	// obj공용 // m_pTexSRV 생성
	m_Tex = LManager<LTexture>::GetInstance().Load(texFileName);

	return true;
}

bool LDXObject::Init()
{
	return true;
}

bool LDXObject::Frame()
{
	return true;
}

bool LDXObject::PreRender()
{
	m_pImmediateContext->IASetInputLayout(m_pVertexLayout);
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	if (m_Shader)
	{
		// VS와PS Shader를 stage에 세팅
		m_Shader->Apply(m_pImmediateContext);
	}

	if (m_Tex)
	{
		// PSSetShaderResources // ps에 m_pTexSRV 세팅
		m_Tex->Apply(m_pImmediateContext);
	}

	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return true;
}

bool LDXObject::Render()
{
	PreRender();
	PostRender();
	return true;
}

bool LDXObject::PostRender()
{
	if (m_pIndexBuffer == nullptr)
	{
		m_pImmediateContext->Draw(m_VertexList.size(), 0);
	}
	else
	{
		m_pImmediateContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		m_pImmediateContext->DrawIndexed(m_IndexList.size(), 0, 0);
	}
	
	return true;
}

bool LDXObject::Release()
{
	if (m_pDevice) m_pDevice->Release();
	if (m_pImmediateContext) m_pImmediateContext->Release();
	if (m_pVertexBuffer) m_pVertexBuffer->Release();
	if (m_pConstantBuffer) m_pConstantBuffer->Release();
	if (m_pVertexLayout) m_pVertexLayout->Release();

	return true;
}
