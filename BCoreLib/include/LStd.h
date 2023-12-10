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
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment (lib, "DirectXTex.lib")

using namespace Microsoft::WRL;

struct T_BOX
{
    TVector3   vPos[8];
    TVector3   vMax;
    TVector3   vMin;
    TVector3   vCenter;
    TVector3   vAxis[3];
    float      fExtent[3];
};

class LRay
{
public:
    TVector3 m_Origin;
    TVector3 m_Direction;
    TVector3 m_InvDir;
    int sign[3];

    LRay() : m_Origin(TVector3(0.0f, 0.0f, 0.0f)), m_Direction(TVector3(0.0f, 0.0f, 1.0f))
    {
        sign[0] = (m_Direction.x < 0.0f) ? 1 : 0;
        sign[1] = (m_Direction.y < 0.0f) ? 1 : 0;
        sign[2] = (m_Direction.z < 0.0f) ? 1 : 0;
    }
    LRay(const TVector3& origin, const TVector3& direction) : m_Origin(origin), m_Direction(direction)
    {
        sign[0] = (m_Direction.x < 0.0f) ? 1 : 0;
        sign[1] = (m_Direction.y < 0.0f) ? 1 : 0;
        sign[2] = (m_Direction.z < 0.0f) ? 1 : 0;
        m_InvDir.x = 1.0f / direction.x;
        m_InvDir.y = 1.0f / direction.y;
        m_InvDir.z = 1.0f / direction.z;
    }
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

