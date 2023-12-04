/*************************
 * 최종수정일 : 2023-12-04
 * 작성자 : 시진
 * 파일명 : CPick.h
 *************************/

#pragma once
#include "LCore.h"

#define MAX_INTERSECTIONS 16

class LCamera;
class LModel;

class LPick
{
public:
	struct INTERSECTION
	{
		DWORD dwFace;				// mesh face that was intersected. 교점이 속한 메쉬.
		FLOAT fBary1, fBary2;		// barycentric coords of intersection. 교점의 무게중심 좌표.
		FLOAT fDist;				// distance from ray origin to intersection. 반직선 원점에서 교점까지의 거리.
		FLOAT tu, tv;				// texture coords of intersection. 교점의 텍스쳐 좌표.
	};

private:
	HWND m_hWnd;
	ComPtr<ID3D11Device> m_pDevice = nullptr;

	TVector3 m_vPickRayDir;			// 반직선 방향
	TVector3 m_vPickRayOrig;		// 반직선 원점

	DWORD m_dwNumIntersections;		// Number of faces intersected.  교점 수.

public:
	LPick(HWND _hWnd, ComPtr<ID3D11Device> _pDevice);
	~LPick();

	LModel* Pick(
		const LCamera& _camera,
		LModel& _model);

private:
	void CalcPickRay(
		const TMatrix& _matProj,
		const TMatrix& _matView,
		const TMatrix& _matWorld);

	LModel* Process(LModel& _model);

	bool IntersectTriangle(
		const TVector3 orig, const TVector3 dir,			// 
		TVector3& v0, TVector3& v1, TVector3& v2,			//
		FLOAT* t, FLOAT* u, FLOAT* v);						//
};

