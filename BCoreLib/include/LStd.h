#pragma once
#define L_PI (3.141592f)
#define T_PI (3.141592f)
#define DegreeToRadian(x)  (x *(T_PI / 180.0f))
#define RadianToDegree(x) (x *(180.0f / T_PI))
#define T_Epsilon 0.001f
// Sub texture types
#define ID_TBASIS_AM 0   // ambient
#define ID_TBASIS_DI 1   // diffuse
#define ID_TBASIS_SP 2   // specular
#define ID_TBASIS_SH 3   // shininesNs
#define ID_TBASIS_SS 4   // shininess strength
#define ID_TBASIS_SI 5   // self-illumination
#define ID_TBASIS_OP 6   // opacity
#define ID_TBASIS_FI 7   // filter color
#define ID_TBASIS_BU 8   // bump 
#define ID_TBASIS_RL 9   // reflection
#define ID_TBASIS_RR 10  // refraction 
#define ID_TBASIS_DP 11  // displacement
#define ID_TBASIS_GN 12  // Generic
#define ID_TBASIS_EV 13  // EnvMap
#define SimpleVertex PNCT_VERTEX
#define TSphere T_SPHERE

#define randstep(fMin,fMax) (fMin+((float)fMax-(float)fMin)*rand()/(float)RAND_MAX)

#define LWRITE  LWrite::GetInstance()
#define LINPUT	LInput::GetInstance()
//마우스 월드좌표
#define MOUSEX  LINPUT.GetWorldPos(LGlobal::g_WindowWidth, LGlobal::g_WindowHeight, LGlobal::g_pMainCamera->m_vCameraPos.x, LGlobal::g_pMainCamera->m_vCameraPos.y).x
#define MOUSEY  LINPUT.GetWorldPos(LGlobal::g_WindowWidth, LGlobal::g_WindowHeight, LGlobal::g_pMainCamera->m_vCameraPos.x, LGlobal::g_pMainCamera->m_vCameraPos.y).y

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
//#include <d3dx11effect.h>


#pragma comment(lib, "d3d11.lib")
//#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment (lib, "DirectXTex.lib")

//#pragma comment(lib, "Effects11d.lib")


using namespace Microsoft::WRL;
using namespace DirectX;
using namespace std;


const enum OBJECTCLASSTYPE {
    CLASS_GEOM = 0,
    CLASS_BONE,
    CLASS_DUMMY,
    CLASS_BIPED,
};

const enum FILETYPE {
    NULLFILE = 0,
    ASEFILE,
    TBSFILE,
    SKMFILE,
    MATFILE,
};

struct TScene
{
    int iVersion;		// 버전
    int	iFirstFrame;    // 시작 프레임
    int	iLastFrame;     // 마지막 프레임
    int	iFrameSpeed;    // 1초당 프레임 개수(30)
    int	iTickPerFrame;  // 1프레임의 틱 값(160)
    int iNumMesh;		// 메쉬오브젝트 개수
    int iMaxWeight;		// 정점 당 가중치
    int iBindPose;		// 바인딩 포즈 에니메이션 여부
};
struct VersionMark
{
    int		iVersion;
    TCHAR	description[128];
};


//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------
struct Rect;

struct PCT_VERTEX
{
	TVector3 p;
	TVector4 c;
	TVector2 t;
	bool operator == (const PCT_VERTEX& Vertex)
	{
		if (p == Vertex.p && c == Vertex.c && t == Vertex.t)
		{
			return true;
		}
		return  false;
	}
	PCT_VERTEX() {}
	PCT_VERTEX(TVector3		vp,
		TVector4		vc,
		TVector2     vt)
	{
		p = vp, c = vc, t = vt;
	}
};
struct PCT2_VERTEX
{
	TVector3 p;
	TVector4 c;
	TVector2 t0;
	TVector2 t1;
	bool operator == (const PCT2_VERTEX& Vertex)
	{
		if (p == Vertex.p && c == Vertex.c && t0 == Vertex.t0 && t1 == Vertex.t1)
		{
			return true;
		}
		return  false;
	}
	PCT2_VERTEX() {}
	PCT2_VERTEX(TVector3		vp,
		TVector4		vc,
		TVector2     vt0,
		TVector2     vt1)
	{
		p = vp, c = vc, t0 = vt0, t1 = vt1;
	}
};
struct PC_VERTEX
{
	TVector3 p;
	TVector4 c;
	bool operator == (const PC_VERTEX& Vertex)
	{
		if (p == Vertex.p && c == Vertex.c)
		{
			return true;
		}
		return  false;
	}
	PC_VERTEX() {}
	PC_VERTEX(TVector3		vp,
		TVector4		vc)
	{
		p = vp, c = vc;
	}
};
struct PNC_VERTEX
{
	TVector3 p;
	TVector3 n;
	TVector4 c;
	bool operator == (const PNC_VERTEX& Vertex)
	{
		if (p == Vertex.p && n == Vertex.n && c == Vertex.c)
		{
			return true;
		}
		return  false;
	}
	PNC_VERTEX() {}
	PNC_VERTEX(TVector3		vp,
		TVector3		vn,
		TVector4		vc)
	{
		p = vp, n = vn, c = vc;
	}
};
struct PNCT_VERTEX
{
	TVector3		p;
	TVector3		n;
	TVector4		c;
	TVector2     t;
	bool operator == (const PNCT_VERTEX& Vertex)
	{
		if (p == Vertex.p && n == Vertex.n && c == Vertex.c && t == Vertex.t)
		{
			return true;
		}
		return  false;
	}
	PNCT_VERTEX() {}
	PNCT_VERTEX(TVector3		vp,
		TVector3		vn,
		TVector4		vc,
		TVector2     vt)
	{
		p = vp, n = vn, c = vc, t = vt;
	}
};
struct PNCT2_VERTEX
{
	TVector3		p;
	TVector3		n;
	TVector4		c;
	TVector2     t;
	TVector3		vTangent;
	bool operator == (const PNCT2_VERTEX& Vertex)
	{
		if (p == Vertex.p && n == Vertex.n && c == Vertex.c && vTangent == Vertex.vTangent)
		{
			return true;
		}
		return  false;
	}
	PNCT2_VERTEX() {}
	PNCT2_VERTEX(TVector3		vp,
		TVector3		vn,
		TVector4		vc,
		TVector2     vt,
		TVector3		tangent)
	{
		p = vp, n = vn, c = vc, t = vt, vTangent = tangent;
	}
};
// ─────────────────────────────────
//  쉐이더 사용시( 가중치 4개 제한 사용 )
// ─────────────────────────────────
struct PNCT3_VERTEX
{
	TVector3		p;
	TVector3		n;
	TVector4		c;
	TVector2		t;
	TVector4		i;	// I0, I1, I2, I3
	TVector4		w;// W0, fW1, fW2, fNumWeight;
	bool operator == (const PNCT3_VERTEX& Vertex)
	{
		if (p == Vertex.p && n == Vertex.n && c == Vertex.c && t == Vertex.t)
		{
			return true;
		}
		return  false;
	}
	PNCT3_VERTEX() {
		w.x = w.y = w.z = w.w = 0.0f;
		i.x = i.y = i.z = i.w = 0.0f;
	}
	PNCT3_VERTEX(TVector3		vp,
		TVector3		vn,
		TVector4		vc,
		TVector2     vt,
		TVector4		vw,
		TVector4		vi)
	{
		p = vp, n = vn, c = vc, t = vt;
		w = vw, i = vi;
	}
};
// ─────────────────────────────────
//  쉐이더 사용시( 가중치 8개 제한 사용 )
// ─────────────────────────────────
struct PNCT5_VERTEX
{
	TVector3		p;
	TVector3		n;
	TVector4		c;
	TVector2		t;
	TVector4		w0;// W0, fW1, fW2, fW3;
	TVector4		i0;	// I0, I1, I2, I3
	TVector4		w1;// W4, fW5, fW6, fNumWeight;	
	TVector4		i1;	// I4, I5, I6, I7
	bool operator == (const PNCT5_VERTEX& Vertex)
	{
		if (p == Vertex.p && n == Vertex.n && c == Vertex.c && t == Vertex.t)
		{
			return true;
		}
		return  false;
	}
	PNCT5_VERTEX() {
		w0.x = w0.y = w0.z = w0.w = 0.0f;
		i0.x = i0.y = i0.z = i0.w = 0.0f;
		w1.x = w1.y = w1.z = w1.w = 0.0f;
		i1.x = i1.y = i1.z = i1.w = 0.0f;
	}
	PNCT5_VERTEX(TVector3		vp,
		TVector3		vn,
		TVector4		vc,
		TVector2     vt,
		TVector4		vw0,
		TVector4		vi0,
		TVector4		vw1,
		TVector4		vi1)
	{
		p = vp, n = vn, c = vc, t = vt;
		w0 = vw0, i0 = vi0, w1 = vw1, i1 = vi1;
	}
};
struct IW_VERTEX
{
	float       t[3];
	float 		i1[4];
	float 		w1[4];
	IW_VERTEX()
	{
		i1[0] = i1[1] = i1[2] = i1[3] = 0.0f;
		w1[0] = w1[1] = w1[2] = w1[3] = 0;
	}
};
//struct TBipedMesh
//{
//	T_STR		m_strName;
//	int					m_iNumFace;
//	TMatrix			m_matWorld;
//	vector<PNC_VERTEX>	m_VertexList;
//	PNC_VERTEX* m_pDrawVertex;
//};

struct VS_CONSTANT_BUFFER
{
	TMatrix matWorld;// c0						
	TMatrix matView;	// c4						
	TMatrix matProj;	// c8						
	TVector4 Color; // 12
};

struct TObjFrame
{
	vector<TMatrix>  m_matWorldList;
	int  				m_iIndex;
	TObjFrame(int iNumMesh, int iIndex)
	{
		m_matWorldList.resize(iNumMesh);
		m_iIndex = iIndex;
	}
};

typedef struct
{
	double x, y, z;
} Point3;




using InstanceID = std::pair<UINT, UINT>;


enum class CollisionType
{
    NONE, // 아무것도 충돌이 없을때
    MAP,
    NPC,
    PLAYER,
};
enum TCollisionType
{
	RECT_OUT = 0,
	RECT_IN,
	RECT_OVERLAP,
};


struct T_PLANE
{
	float	fA, fB, fC, fD;
	bool	CreatePlane(TVector3 v0, TVector3 v1, TVector3 v2)
	{
		TVector3 vEdge0 = v1 - v0;
		TVector3 vEdge1 = v2 - v0;
		TVector3 vNormal;
		D3DXVec3Cross(&vNormal, &vEdge0, &vEdge1);
		D3DXVec3Normalize(&vNormal, &vNormal);

		fA = vNormal.x;	fB = vNormal.y;	fC = vNormal.z;
		fD = -(fA * v0.x + fB * v0.y + fC * v0.z);
		return true;
	}
	bool	CreatePlane(TVector3 vNormal, TVector3 v0)
	{
		D3DXVec3Normalize(&vNormal, &vNormal);

		fA = vNormal.x;	fB = vNormal.y;	fC = vNormal.z;
		fD = -(fA * v0.x + fB * v0.y + fC * v0.z);
		return true;
	}
	void	Normalize()
	{
		float fMag = sqrt(fA * fA + fB * fB + fC * fC);
		fA = fA / fMag;
		fB = fB / fMag;
		fC = fC / fMag;
		fD = fD / fMag;
	}

};
struct T_SPHERE
{
	TVector3 vCenter;
	float fRadius;
	T_SPHERE() {};
	T_SPHERE(TVector3 vC, float r) {
		vCenter = vC;
		fRadius = r;
	};
};

struct T_BOX
{
    TVector3   vPos[8];
    TVector3   vMax;
    TVector3   vMin;
    TVector3   vCenter;
    TVector3   vAxis[3];
    float      fExtent[3];
	T_BOX()
	{

	}
	T_BOX(TVector3	max, TVector3		min)
	{
		Set(max, min);
	}
	void Set(TVector3	max, TVector3	min)
	{
		vMax = max;
		vMin = min;
		vCenter = (vMax + vMin) * 0.5f;
		vAxis[0] = { 1,0,0 };
		vAxis[1] = { 0,1,0 };
		vAxis[2] = { 0,0,1 };
		fExtent[0] = vMax.x - vCenter.x;
		fExtent[1] = vMax.y - vCenter.y;
		fExtent[2] = vMax.z - vCenter.z;
	}
};

struct TCircle
{
	float cx;
	float cy;
	float fRadius;
	TCircle() {};
	TCircle(float x, float y, float r) {
		cx = x;
		cy = y;
		fRadius = r;
	};
};



struct TRect
{
	float  x1;
	float  y1;
	float  w;
	float  h;

	float  x2;//x+w
	float  y2;//y+h
	float  cx;
	float  cy;
	bool   operator == (TRect& dest)
	{
		if (fabs(x1 - dest.x1) < T_Epsilon)
		{
			if (fabs(y1 - dest.y1) < T_Epsilon)
			{
				if (fabs(w - dest.w) < T_Epsilon)
				{
					if (fabs(h - dest.h) < T_Epsilon)
					{
						return true;
					}
				}
			}
		}
		return false;
	}
	TRect() {}
	TRect(float x, float y, float z, float w) {
		Set(x, y, z, w);
	}
	void  Set(float x, float y, float z, float w)
	{
		this->x1 = x;
		this->y1 = y;
		this->w = z;
		this->h = w;
		x2 = this->x1 + this->w;
		y2 = this->y1 + this->h;
		cx = (this->x1 + x2) / 2.0f;
		cy = (this->y1 + y2) / 2.0f;
	}
};

class TCollision
{
public:
	static TCollisionType   RectToRect(TRect& a, TRect& b, TRect& Intersect)
	{
		// 0 : 완전제외(0)
	// 1 : 완전포함(1) -> 걸쳐져 있는 상태(2)
	// a.x(10)----30------a.x2(40)
	// 합집합
		float fMinX;   float fMinY;
		float fMaxX;   float fMaxY;
		fMinX = a.x1 < b.x1 ? a.x1 : b.x1;
		fMinY = a.y1 < b.y1 ? a.y1 : b.y1;
		fMaxX = a.x2 > b.x2 ? a.x2 : b.x2;
		fMaxY = a.y2 > b.y2 ? a.y2 : b.y2;
		//  가로 판정
		if ((a.w + b.w) >= (fMaxX - fMinX))
		{
			//  세로 판정
			if ((a.h + b.h) >= (fMaxY - fMinY))
			{
				// 교차한다. 교집합  
				float x, y, x2, y2;
				x = a.x1 > b.x1 ? a.x1 : b.x1;
				y = a.y1 > b.y1 ? a.y1 : b.y1;
				x2 = a.x2 < b.x2 ? a.x2 : b.x2;
				y2 = a.y2 < b.y2 ? a.y2 : b.y2;
				Intersect.Set(x, y, x2 - x, y2 - y);
				if (Intersect == a || Intersect == b)
				{
					return TCollisionType::RECT_IN;
				}

				return TCollisionType::RECT_OVERLAP;
			}
		}
		return TCollisionType::RECT_OUT;
	}
	static bool             RectToInRect(TRect& a, TRect& b)
	{
		//  |             |
		if (a.x1 <= b.x1)
		{
			if ((a.x1 + a.w) >= b.x1 + b.w)
			{
				if (a.y1 <= b.y1)
				{
					if ((a.y1 + a.h) >= b.y1 + b.h)
					{
						return true;
					}
				}
			}
		}
		return false;
	}
	static bool             RectToPoint(TRect& a, POINT& p)
	{
		if (a.x1 <= p.x && a.x2 >= p.x)
		{
			if (a.y1 <= p.y && a.y2 >= p.y)
			{
				return true;
			}
		}
		return false;
	}
	static bool             CircleToCircle(TCircle& a, TCircle& b)
	{
		float fSumRadius = a.fRadius + b.fRadius;
		float x = a.cx - b.cx;
		float y = a.cy - b.cy;
		float fDistance = sqrt(x * x + y * y);
		if (fDistance <= fSumRadius)
		{
			return true;
		}
		return false;
	}
public:
	static TCollisionType   BoxToBox(T_BOX& a, T_BOX& b)
	{
		// 0 : 완전제외(0)
	// 1 : 완전포함(1) -> 걸쳐져 있는 상태(2)
	// a.x(10)----30------a.x2(40)
	// 합집합
		float fMinX;    float fMaxX;
		float fMinY;    float fMaxY;
		float fMinZ;    float fMaxZ;
		fMinX = a.vMin.x < b.vMin.x ? a.vMin.x : b.vMin.x;
		fMinY = a.vMin.y < b.vMin.y ? a.vMin.y : b.vMin.y;
		fMaxX = a.vMax.x > b.vMax.x ? a.vMax.x : b.vMax.x;
		fMaxY = a.vMax.y > b.vMax.y ? a.vMax.y : b.vMax.y;

		fMinZ = a.vMin.z < b.vMin.z ? a.vMin.z : b.vMin.z;
		fMaxZ = a.vMax.z > b.vMax.z ? a.vMax.z : b.vMax.z;

		TVector3 vSize;
		vSize.x = (a.vMax.x - a.vMin.x) + (b.vMax.x - b.vMin.x);
		vSize.y = (a.vMax.y - a.vMin.y) + (b.vMax.y - b.vMin.y);
		vSize.z = (a.vMax.z - a.vMin.z) + (b.vMax.z - b.vMin.z);

		//  가로 판정
		if (vSize.x >= (fMaxX - fMinX))
		{        //  세로 판정
			if (vSize.y >= (fMaxY - fMinY))
			{
				if (vSize.z >= (fMaxZ - fMinZ))
				{
					// 교차한다. 교집합
					TVector3 vMin, vMax;
					T_BOX Intersect;
					vMin.x = a.vMin.x > b.vMin.x ? a.vMin.x : b.vMin.x;
					vMin.y = a.vMin.y > b.vMin.y ? a.vMin.y : b.vMin.y;
					vMin.z = a.vMin.z > b.vMin.z ? a.vMin.z : b.vMin.z;

					vMax.x = a.vMax.x < b.vMax.x ? a.vMax.x : b.vMax.x;
					vMax.y = a.vMax.y < b.vMax.y ? a.vMax.y : b.vMax.y;
					vMax.z = a.vMax.z < b.vMax.z ? a.vMax.z : b.vMax.z;

					if (BoxToInBox(a, b))
					{
						return TCollisionType::RECT_IN;
					}
					return TCollisionType::RECT_OVERLAP;
				}
			}
		}
		return TCollisionType::RECT_OUT;
	}
	static bool             BoxToInBox(T_BOX& a, T_BOX& b)
	{
		//  |             |
		if (a.vMin.x <= b.vMin.x &&
			a.vMin.y <= b.vMin.y &&
			a.vMin.z <= b.vMin.z)
		{
			if (a.vMax.x >= b.vMax.x &&
				a.vMax.y >= b.vMax.y &&
				a.vMax.z >= b.vMax.z)
			{
				return true;
			}
		}
		return false;
	}
	static bool             BoxToPosition(T_BOX& a, TVector3& p)
	{
		//  |             |
		if (a.vMin.x <= p.x &&
			a.vMin.y <= p.y &&
			a.vMin.z <= p.z)
		{
			if (a.vMax.x >= p.x &&
				a.vMax.y >= p.y &&
				a.vMax.z >= p.z)
			{
				return true;
			}
		}
		return false;
	}
	static bool             SphereToSphere(TSphere& a, TSphere& b)
	{
		float fSumRadius = a.fRadius + b.fRadius;
		TVector3 vDir = a.vCenter - b.vCenter;
		float fDistance = vDir.Length();
		if (fDistance <= fSumRadius)
		{
			return true;
		}
		return false;
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

//struct SimpleVertex
//{
//    TVector3 p;
//    TVector3 n;
//    TVector4 c;
//    TVector2 t;
//
//    SimpleVertex() = default;
//    SimpleVertex(const TVector3 & pos, const TVector2 & uv, const TVector4 & color, const TVector3 & normal) : p(pos), t(uv), n(normal), c(color) {}
//};





//////////////////////////////////////////////
// 객체 및 배열 할당과 삭제 및 소멸 매크로
//////////////////////////////////////////////
#ifndef SAFE_ZERO
#define SAFE_ZERO(A)				{ A = 0; }
#endif

#ifndef SAFE_NEW
#define SAFE_NEW(A, B)				{ if (!A) A = new B; }
#endif 

#ifndef SAFE_DEL
#define SAFE_DEL(A)					{ if (A) delete A; (A)=NULL; }
#endif 

#ifndef SAFE_NEW_ARRAY
#define SAFE_NEW_ARRAY(A, B, C)		{ if (!A && C) A = new B[C]; }
#endif 

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(A)		{ if (A) delete [] A; (A)=NULL; }
#endif 

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(A)				{ if(A) { (A)->Release(); (A)=NULL; } }
#endif 

#ifndef SAFE_NEW_CLEAR
#define SAFE_NEW_CLEAR( A, B )			{ if (!A) A = new B; if(A) memset( A, 0, sizeof(B) ); };
#endif 

#ifndef SAFE_NEW_ARRAY_CLEAR
#define SAFE_NEW_ARRAY_CLEAR( A, B, C )	{ if (!A && C) A = new B[C]; if(A) memset( A, 0, sizeof(B)*C ); };
#endif
