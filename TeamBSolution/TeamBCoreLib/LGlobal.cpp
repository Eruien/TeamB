#include "LGlobal.h"
HWND LGlobal::g_hWnd = nullptr;
int LGlobal::g_WindowWidth = 1280;
int LGlobal::g_WindowHeight = 720;
LWindow* LGlobal::g_pWindow = nullptr;
ComPtr<ID3D11Device> LGlobal::g_pDevice;
ComPtr<ID3D11DeviceContext> LGlobal::g_pImmediateContext;
ComPtr<IDXGISwapChain> LGlobal::g_pSwapChain;
ComPtr<ID3D11DepthStencilState> LGlobal::g_pDepthStencilState;
ComPtr<ID3D11DepthStencilState> LGlobal::g_pDepthStencilStateDisable;
float LGlobal::g_fSPF = 0;
float LGlobal::g_fGameTimer = 0;
LCamera* LGlobal::g_pMainCamera = nullptr;
UICamera* LGlobal::g_pUICamera = nullptr;
HINSTANCE LGlobal::g_hInstance = nullptr;
LPlayer* LGlobal::g_PlayerModel = nullptr;
LSound* LGlobal::g_BackgroundSound = nullptr;
LSound* LGlobal::g_IngameSound = nullptr;
LSound* LGlobal::g_EffectSound1 = nullptr;
LSound* LGlobal::g_EffectSound2 = nullptr;
LSound* LGlobal::g_SteamPackSound = nullptr;
LSound* LGlobal::g_ZedTimeStart = nullptr;
LSound* LGlobal::g_ZedTimeEnd = nullptr;
LSound* LGlobal::g_PlayerHitSound = nullptr;;
LSound* LGlobal::g_HeadShotSound = nullptr;
LSound* LGlobal::g_KillSound = nullptr;