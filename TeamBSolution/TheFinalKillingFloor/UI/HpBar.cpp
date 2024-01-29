#include "HpBar.h"
#include "KObject.h"
#include "LGlobal.h"
HpBar::HpBar() : MonoBehaviour(L"HpBar")
{

}


HpBar::~HpBar()
{
}

void HpBar::CreateConstantBuffer()
{

  	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	bufferDesc.ByteWidth = sizeof(cbData_hp);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	HRESULT hr = GetGameObject()->m_pDevice->CreateBuffer(
		&bufferDesc,
		nullptr,
		_cbBuffer.GetAddressOf());

	if (FAILED(hr))
	{
		MessageBoxA(NULL, "Create ConstantBuffer Error", "Error Box", MB_OK);
		return;
	}

	return;
}

void HpBar::Init()
{

	GetGameObject()->m_Shader->LoadPixelShader(L"../../res/hlsl/CustomizeMap.hlsl", "PS_hp");
	CreateConstantBuffer();
}

void HpBar::Frame()
{
	UpdateHp(50.0f);
}

void HpBar::UpdateHp(float hp)
{

	cbData_hp cb1;
	cb1.hp = hp;
	GetGameObject()->m_pImmediateContext->UpdateSubresource(GetGameObject()->m_pConstantBuffer.Get(), 0, NULL, &cb1, 0, 0);
}
