#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include "LCamera.h"

class LGlobal
{
public:
	static ID3D11Device* g_pDevice;
	static ID3D11DeviceContext* g_pImmediateContext;
	static LCamera* g_pMainCamera;
};

