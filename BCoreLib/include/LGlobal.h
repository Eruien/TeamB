#pragma once
#include "LStd.h"
#include "LCamera.h"

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
	static float g_fSPF;
	static float g_fGameTimer;
	static LCamera* g_pMainCamera;
};


