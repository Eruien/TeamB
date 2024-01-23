#pragma once
#include "LDevice.h"
#include "LManager.h"
#include "LManagerAgr.h"
#include "LDebugCamera.h"
#include "LTimer.h"
#include "LInput.h"
#include "LWrite.h"
#include "LEffect.h"


class LCore : public LDevice
{
public:
	std::shared_ptr<LTimer> m_Gametimer = nullptr;
	std::shared_ptr<LCamera> m_pDefaultCamera = nullptr;
	ComPtr<ID3D11BlendState> m_AlphaBlend;
	ComPtr<ID3D11SamplerState> m_pSamplerState;
	ComPtr<ID3D11DepthStencilState> m_pDepthStencilState;
	ComPtr<ID3D11DepthStencilState> m_pDepthStencilStateDisable;
	ComPtr<ID3D11RasterizerState> m_pRSWireFrame;
	ComPtr<ID3D11RasterizerState> m_pRSSolid;
	bool m_ISWireFrame = false;
public:
	void CreateBlendState();
	void CreateSamplerState();
	void CreateDepthStencilState();
	void CreateRasterizerState();
public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();
	bool Run();
private:
	bool EngineInit();
	bool EngineFrame();
	bool EngineRender();
	bool EngineRelease();
public:
	virtual ~LCore();
};

