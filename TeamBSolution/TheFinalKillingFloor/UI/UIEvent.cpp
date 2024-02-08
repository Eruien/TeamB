#include "UIEvent.h"
#include "KObject.h"
#include "LGlobal.h"
UIEvent::UIEvent(wstring Function) : MonoBehaviour(L"UIEvent")
{
	_function = Function;
	_functionMap[L"HitPlayerEffect"] = &UIEvent::HitPlayerEffect;

}

UIEvent::~UIEvent()
{
}

void UIEvent::Init()
{
	//if(_function == L"HitPlayerEffect")
	//{
	//GetGameObject()->Create(L"../../res/hlsl/Fade.hlsl", L"../../res/ui/GoreSplash.png");
	//CreateConstantBuffer();
	//}
	GetGameObject()->Create(L"../../res/hlsl/Fade.hlsl", L"../../res/ui/GoreSplash.png");
	CreateConstantBuffer();
}

void UIEvent::Frame()
{
	if (_functionMap.find(_function) != _functionMap.end())
	{
			(this->*_functionMap[_function])();
	}
	else {
		std::cout << "Function not found" << std::endl;
	}


}

void UIEvent::Render()
{


}

void UIEvent::HitPlayerEffect()
{
	if (GetGameObject()->GetIsRender())
	{
		static float temp = 0; // ÀÌµû°¡ ¸â¹ö·Î »©»ï
		temp += LGlobal::g_fSPF / _duration;
		data.alpha[0] = temp;

		LGlobal::g_pImmediateContext->UpdateSubresource(_cBuff.Get(), 0, NULL, &data, 0, 0);
		LGlobal::g_pImmediateContext->PSSetConstantBuffers(5, 1, _cBuff.GetAddressOf());
		if (data.alpha[0] >= 1.0f)
		{
			//data.alpha[0] = 0;
			temp = 0;
			GetGameObject()->SetIsRender(false);

		}
	}
}

void UIEvent::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC bufferDesc{};
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	bufferDesc.ByteWidth = sizeof(cb_DataA);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	HRESULT hr = LGlobal::g_pDevice->CreateBuffer(
		&bufferDesc,
		nullptr,
		_cBuff.GetAddressOf());

}
