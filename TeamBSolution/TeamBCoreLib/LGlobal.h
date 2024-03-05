#pragma once
#include "LStd.h"
#include "LCamera.h"
#include "UICamera.h"
#include "LSoundMgr.h"

class LWindow;
class LModel;
class LSkinningModel;
class LPlayer;

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
	static LPlayer* g_PlayerModel;
	static LSound* g_BackgroundSound;
	static LSound* g_PlayerHitSound;
	static LSound* g_IngameSound;
	static LSound* g_EffectSound1;
	static LSound* g_EffectSound2;
	static LSound* g_SteamPackSound;
	static LSound* g_ZedTimeStart;
	static LSound* g_ZedTimeEnd;
	static LSound* g_HeadShotSound;
	static LSound* g_KillSound;

};


