#pragma once
#include "LStd.h"
#include "LCamera.h"
#include "UICamera.h"
class LWindow;
class LModel;
class LSkinningModel;


class LGlobal
{
public:
	static HWND g_hWnd;
	static HINSTANCE g_hInstance;
	static int g_WindowWidth;
	static int g_WindowHeight;
	static LWindow* g_pWindow;
	static ComPtr<ID3D11Device> g_pDevice;
	static ComPtr<ID3D11DeviceContext> g_pImmediateContext;
	static ComPtr<ID3D11DepthStencilState> g_pDepthStencilState;
	static ComPtr<ID3D11DepthStencilState> g_pDepthStencilStateDisable;
	static float g_fSPF;
	static float g_fGameTimer;
	static LCamera* g_pMainCamera;
	static UICamera* g_pUICamera;
	static float g_HP;
	static int g_BulletCount;
};


