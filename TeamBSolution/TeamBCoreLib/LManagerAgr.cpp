#include "LManagerAgr.h"
#include "LGlobal.h"

//Texture
void LTexture::Apply()
{	
	LGlobal::g_pImmediateContext->PSSetShaderResources(0, 1, &m_pTexSRV);
}

bool LTexture::Load(std::wstring fileName)
{
	m_texPath = fileName;
	auto imageobj = std::make_unique<DirectX::ScratchImage>();
	DirectX::TexMetadata mdata;

	HRESULT hr = DirectX::GetMetadataFromWICFile(fileName.c_str(), DirectX::WIC_FLAGS_NONE, mdata);
	hr = DirectX::LoadFromWICFile(fileName.c_str(), DirectX::WIC_FLAGS_NONE, &mdata, *imageobj);
	hr = DirectX::CreateShaderResourceView(LGlobal::g_pDevice.Get(), imageobj->GetImages(), imageobj->GetImageCount(), mdata, &m_pTexSRV);

	if (SUCCEEDED(hr))
	{
		return true;
	}

	if (FAILED(hr))
	{
		MessageBoxA(NULL, "Create ShaderResourceView WIC Error", "Error Box", MB_OK);
	}

	hr = DirectX::GetMetadataFromDDSFile(fileName.c_str(), DirectX::DDS_FLAGS_NONE, mdata);
	hr = DirectX::LoadFromDDSFile(fileName.c_str(), DirectX::DDS_FLAGS_NONE, &mdata, *imageobj);
	hr = DirectX::CreateShaderResourceView(LGlobal::g_pDevice.Get(), imageobj->GetImages(), imageobj->GetImageCount(), mdata, &m_pTexSRV);
	if (SUCCEEDED(hr))
	{
		return true;
	}

	if (FAILED(hr))
	{
		MessageBoxA(NULL, "Create ShaderResourceView DDS Error", "Error Box", MB_OK);
	}

	hr = DirectX::GetMetadataFromTGAFile(fileName.c_str(), DirectX::TGA_FLAGS_NONE, mdata);
	hr = DirectX::LoadFromTGAFile(fileName.c_str(), DirectX::TGA_FLAGS_NONE, &mdata, *imageobj);
	hr = DirectX::CreateShaderResourceView(LGlobal::g_pDevice.Get(), imageobj->GetImages(), imageobj->GetImageCount(), mdata, &m_pTexSRV);

	if (SUCCEEDED(hr))
	{
		return true;
	}

	if (FAILED(hr))
	{
		MessageBoxA(NULL, "Create ShaderResourceView TGA Error", "Error Box", MB_OK);
	}
	
	return true;
}

bool LTexture::Release()
{
	if (m_pTexSRV) m_pTexSRV->Release();
	m_pTexSRV = nullptr;
	return true;
}

LTexture::LTexture() {}
LTexture::~LTexture() {}

//Shader
void LShader::Apply()
{
	LGlobal::g_pImmediateContext->VSSetShader(m_pVS, NULL, 0);
	LGlobal::g_pImmediateContext->HSSetShader(NULL, NULL, 0);
	LGlobal::g_pImmediateContext->DSSetShader(NULL, NULL, 0);
	LGlobal::g_pImmediateContext->GSSetShader(NULL, NULL, 0);
	LGlobal::g_pImmediateContext->PSSetShader(m_pPS, NULL, 0);
}

bool LShader::Load(std::wstring fileName)
{
	LoadVertexShader(fileName);
	LoadPixelShader(fileName);

	return true;
}

bool LShader::LoadVertexShader(std::wstring fileName)
{
	HRESULT hr;
	ID3DBlob* ErrorCode;

	hr = D3DCompileFromFile(
		fileName.c_str(),
		NULL,
		NULL,
		"VS",
		"vs_5_0",
		NULL,
		NULL,
		&m_pVSBlob,
		&ErrorCode);

	if (FAILED(hr))
	{
		//ErrorCode
		LPCSTR pMessage[500] = { 0, };
		MessageBoxA(NULL, *pMessage, "Error Box", MB_OK);
		return false;
	}

	hr = LGlobal::g_pDevice->CreateVertexShader(
		m_pVSBlob->GetBufferPointer(),
		m_pVSBlob->GetBufferSize(),
		NULL,
		&m_pVS);

	if (FAILED(hr))
	{
		MessageBoxA(NULL, "Create VertexShader Error", "Error Box", MB_OK);
		return false;
	}

	if (ErrorCode) ErrorCode->Release();

	return true;
}

bool LShader::LoadPixelShader(std::wstring fileName)
{
	ID3DBlob* ErrorCode;
	HRESULT hr;

	hr = D3DCompileFromFile(
		fileName.c_str(),
		NULL,
		NULL,
		"PS",
		"ps_5_0",
		NULL,
		NULL,
		&m_pPSBlob,
		&ErrorCode);

	if (FAILED(hr))
	{
		LPCSTR pMessage[500] = { 0, };
		MessageBoxA(NULL, *pMessage, "Error Box", MB_OK);
		return false;
	}

	hr = LGlobal::g_pDevice->CreatePixelShader(
		m_pPSBlob->GetBufferPointer(),
		m_pPSBlob->GetBufferSize(),
		NULL,
		&m_pPS);

	if (FAILED(hr))
	{
		MessageBoxA(NULL, "Create PixelShader Error", "Error Box", MB_OK);
		return false;
	}

	return true;
}

bool LShader::Release()
{
	if(m_pVSBlob) m_pVSBlob->Release();
	if(m_pPSBlob) m_pPSBlob->Release();
	if(m_pVS) m_pVS->Release();
	if(m_pPS) m_pPS->Release();
	m_pVSBlob = nullptr;
	m_pPSBlob = nullptr;
	m_pVS = nullptr;
	m_pPS = nullptr;

	return true;
}

LShader::LShader() {}
LShader::~LShader() {}