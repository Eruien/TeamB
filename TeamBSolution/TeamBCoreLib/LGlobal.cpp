#include "LGlobal.h"
HWND LGlobal::g_hWnd = nullptr;
int LGlobal::g_WindowWidth = 1280;
int LGlobal::g_WindowHeight = 720;
LWindow* LGlobal::g_pWindow = nullptr;
ComPtr<ID3D11Device> LGlobal::g_pDevice;
ComPtr<ID3D11DeviceContext> LGlobal::g_pImmediateContext;
ComPtr<ID3D11DepthStencilState> LGlobal::g_pDepthStencilState;
ComPtr<ID3D11DepthStencilState> LGlobal::g_pDepthStencilStateDisable;
float LGlobal::g_fSPF = 0;
float LGlobal::g_fGameTimer = 0;
LCamera* LGlobal::g_pMainCamera = nullptr;
UICamera* LGlobal::g_pUICamera = nullptr;
HINSTANCE LGlobal::g_hInstance = nullptr;
float LGlobal::g_HP = 100.0f;
int LGlobal::g_BulletCount = 30;