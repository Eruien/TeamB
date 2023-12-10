#include "LGlobal.h"

HWND LGlobal::g_hWnd = nullptr;
int LGlobal::g_WindowWidth = 0;
int LGlobal::g_WindowHeight = 0;
ComPtr<ID3D11Device> LGlobal::g_pDevice;
ComPtr<ID3D11DeviceContext> LGlobal::g_pImmediateContext;
float LGlobal::g_fSPF = 0;
float LGlobal::g_fGameTimer = 0;
LCamera* LGlobal::g_pMainCamera = nullptr;

