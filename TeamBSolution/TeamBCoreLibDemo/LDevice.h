#pragma once
#include "LWindow.h"

class LDevice : public LWindow
{
public:
	// CreateDevice
	D3D_DRIVER_TYPE m_DriverType = D3D_DRIVER_TYPE_HARDWARE;
	D3D_FEATURE_LEVEL m_FeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D11_VIEWPORT m_ViewPort = { 0, };
	// interface
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pImmediateContext = nullptr;
	IDXGISwapChain* m_pSwapChain = nullptr;
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;
	// Release에서 말고 다른것이 생성될 때 자체적으로 메모리 해제
	IDXGIFactory* m_pGIFactory = nullptr;
	ID3D11Texture2D* m_pbackBuffer = nullptr;
public:
	ComPtr<ID3D11DepthStencilView> m_pDepthStencilView = nullptr;
	ComPtr<ID3D11Texture2D> m_DepthTexture = nullptr;
public:
	bool SetDevice();
	bool SetGIFactory();
	bool SetSwapChain();
	bool SetDepthTexture();
	bool SetDepthStencilView();
	bool SetRenderTargetView();
	bool SetViewPort();
public:
	bool DeviceInit();
	bool DeviceFrame();
	bool DevicePreRender();
	bool DevicePostRender();
	bool DeviceRelease();
public:
	virtual ~LDevice();
};

