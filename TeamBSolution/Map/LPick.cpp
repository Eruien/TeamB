#include "LPick.h"
#include "LCamera.h"
#include "LModel.h"

LPick::LPick(HWND _hWnd, ComPtr<ID3D11Device> _pDevice) :
	m_hWnd(_hWnd),
	m_pDevice(_pDevice),
	m_vPickRayDir(0.0f, 0.0f, 0.0f),
	m_vPickRayOrig(0.f, 0.f, 0.f),
	m_dwNumIntersections(0L)
{
}

LPick::~LPick()
{
}

LModel* LPick::Pick(
	const LCamera& _camera,
	LModel& _model)
{
	CalcPickRay(
		_camera.m_matProj,
		_camera.m_matView,
		_model.m_matWorld);
	return Process(_model);
}

void LPick::CalcPickRay(
	const TMatrix& _matProj,
	const TMatrix& _matView,
	const TMatrix& _matWorld)
{
	POINT ptCursor;
	GetCursorPos(&ptCursor);					// 커서 위치 획득
	ScreenToClient(m_hWnd, &ptCursor);			// 스크린 좌표를 클라이언트 좌표로 변환
	
	RECT clientRect;
	GetClientRect(m_hWnd, &clientRect);			// 클라이언트 크기 획득

	// Compute the vector of the Pick Ray in screen space. 
	TVector3 v;
	v.x = (((2.f * ptCursor.x) / clientRect.right) - 1) / _matProj._11;
	v.y = -(((2.f * ptCursor.y) / clientRect.bottom) - 1) / _matProj._22;
	v.z = 1.f;

	// Get the inverse view matrix
	TMatrix mWorldView = _matWorld * _matView;
	TMatrix m;
	D3DXMatrixInverse(&m, NULL, &mWorldView);

	// Transform the screen space Pick ray into 3D space
	m_vPickRayDir.x = v.x * m._11 + v.y * m._21 + v.z * m._31;
	m_vPickRayDir.y = v.x * m._12 + v.y * m._22 + v.z * m._32;
	m_vPickRayDir.z = v.x * m._13 + v.y * m._23 + v.z * m._33;
	m_vPickRayOrig.x = m._41;
	m_vPickRayOrig.y = m._42;
	m_vPickRayOrig.z = m._43;
}

LModel* LPick::Process(LModel& _model)
{
	// Not using D3DX
	DWORD dwNumFaces = 1L;
	FLOAT fBary1, fBary2;
	FLOAT fDist;

	bool g_bALLHits = true; // Whether to just get the first "hit" or all "hits"
	INTERSECTION g_IntersectionArray[MAX_INTERSECTIONS];
	m_dwNumIntersections = 0L;
	
	// *******우선 삼각형 하나에 대해서만 검사. 차후 면이 여러개인 메쉬의 경우 그에 맞춰 처리해야함.*******

	for (DWORD i = 0; i < dwNumFaces; ++i)
	{
		for (DWORD j = 0; j < _model.m_VertexList.size(); j = j + 3)
		{
			TVector3 v0 = _model.m_VertexList[i].p;
			TVector3 v1 = _model.m_VertexList[i + 1].p;
			TVector3 v2 = _model.m_VertexList[i + 2].p;

			if (IntersectTriangle(m_vPickRayOrig, m_vPickRayDir, v0, v1, v2,
				&fDist, &fBary1, &fBary2))
			{
				if (g_bALLHits || m_dwNumIntersections == 0 || fDist < g_IntersectionArray[0].fDist)
				{
					if (!g_bALLHits)
						m_dwNumIntersections = 0L;
					// !삼각형 인덱스라면
					g_IntersectionArray[m_dwNumIntersections].dwFace = 0L;
					g_IntersectionArray[m_dwNumIntersections].fBary1 = fBary1;
					g_IntersectionArray[m_dwNumIntersections].fBary2 = fBary2;
					g_IntersectionArray[m_dwNumIntersections].fDist = fDist;
					m_dwNumIntersections++;
					// 배열이 가득 찼을 때
					if (m_dwNumIntersections == MAX_INTERSECTIONS)
						break;
				}
				return &_model;
			}
		}
	}

	return nullptr;
}

bool LPick::IntersectTriangle(
	const TVector3 orig, const TVector3 dir,
	TVector3& v0, TVector3& v1, TVector3& v2,
	FLOAT* t, FLOAT* u, FLOAT* v)
{
	// Find vectors for two edges sharing vert0
	TVector3 edge1 = v1 - v0;
	TVector3 edge2 = v2 - v0;

	// Begin calculating determinant - also used to calculate U parameter
	TVector3 pvec;								
	D3DXVec3Cross(&pvec, &dir, &edge2);

	// If determinant is near zero, ray lies in plane of triangle. det가 거의 0이라면 직선과 평행.
	FLOAT det = D3DXVec3Dot(&edge1, &pvec);
	
	TVector3 tvec;
	if (det > 0)
	{
		tvec = orig - v0;
	}
	else
	{
		tvec = v0 - orig;
		det = -det;
	}

	if (det < 0.0001f)
		return FALSE;

	// Calculate U parameter and test bounds
	*u = D3DXVec3Dot(&tvec, &pvec);
	if (*u < 0.0f || *u > det)					// 예외처리1. parameter U는 0에서 det사이여야함. 1이 아닌 이유는 1/det 계산 이전이기 때문.
		return false;

	// Prepare to test V parameter
	TVector3 qvec;
	D3DXVec3Cross( OUT &qvec, &tvec, &edge1);
	
	// Calculate V parameter and test bounds
	*v = D3DXVec3Dot(&dir, &qvec);
	if (*v < 0.0f || *u + *v > det)
		return FALSE;

	// Calculate t, scale parameters, ray intersects triangle
	*t = D3DXVec3Dot(&edge2, &qvec);
	FLOAT fInvDet = 1.0f / det;
	*t *= fInvDet;
	*u *= fInvDet;
	*v *= fInvDet;


	return TRUE;
}
