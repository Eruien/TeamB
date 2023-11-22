#pragma once
#include "LDevice.h"
#include "LTimer.h"
#include "LInput.h"
#include "LCamera.h"
#include "LManager.h"
#include "LManagerAgr.h"
#include "LWrite.h"

class LCore : public LDevice
{
public:
	LTimer m_Gametimer;
	std::shared_ptr<LCamera> m_pDefaultCamera = nullptr;
	ComPtr<ID3D11SamplerState> m_pSamplerState;
	ComPtr<ID3D11DepthStencilState> m_pDepthStencilState;
	ComPtr<ID3D11DepthStencilState> m_pDepthStencilStateDisable;
	ComPtr<ID3D11RasterizerState> m_pRSWireFrame;
	ComPtr<ID3D11RasterizerState> m_pRSSolid;
	bool m_ISWireFrame = false;
public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();
	void CreateSamplerState();
	void CreateDepthStencilState();
	void CreateRasterizerState();
	bool Run();
private:
	bool EngineInit();
	bool EngineFrame();
	bool EngineRender();
	bool EngineRelease();
public:
	virtual ~LCore();
};

