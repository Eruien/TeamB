#pragma once
#define L_PI (3.141592f)


#include <windows.h>
#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <list>
#include <queue>
#include <codecvt>
#include <DirectXMath.h>
#include <wrl.h>
//#include "LUtils.h"
#include "TMath.h"

#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#include "DirectXTex.h"
#include <d3dx11effect.h>


#pragma comment(lib, "d3d11.lib")
//#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment (lib, "DirectXTex.lib")

#pragma comment(lib, "Effects11d.lib")


using namespace Microsoft::WRL;
using namespace DirectX;
using namespace std;



using InstanceID = std::pair<UINT, UINT>;


enum class CollisionType
{
    NONE, // 아무것도 충돌이 없을때
    MAP,
    NPC,
    PLAYER,
};

struct T_BOX
{
    TVector3   vPos[8];
    TVector3   vMax;
    TVector3   vMin;
    TVector3   vCenter;
    TVector3   vAxis[3];
    float      fExtent[3];
};

static void L_DebugString(const WCHAR* msg)
{
#ifdef _DEBUG
	OutputDebugString(msg);
#endif
}

static std::wstring mtw(std::string str)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
    return conv.from_bytes(str);
}

static std::string wtm(std::wstring str)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
	return conv.to_bytes(str);
}

struct SimpleVertex
{
    TVector3 p;
    TVector3 n;
    TVector4 c;
    TVector2 t;

    SimpleVertex() = default;
    SimpleVertex(const TVector3 & pos, const TVector2 & uv, const TVector4 & color, const TVector3 & normal) : p(pos), t(uv), n(normal), c(color) {}
};
