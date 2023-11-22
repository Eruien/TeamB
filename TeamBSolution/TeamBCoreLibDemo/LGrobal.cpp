#include "LGlobal.h"

ID3D11Device* LGlobal::g_pDevice = nullptr;
ID3D11DeviceContext* LGlobal::g_pImmediateContext = nullptr;
LCamera* LGlobal::g_pMainCamera = nullptr;