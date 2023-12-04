/*************************
 * ���������� : 2023-12-04
 * �ۼ��� : ����
 * ���ϸ� : CPick.h
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
		DWORD dwFace;				// mesh face that was intersected. ������ ���� �޽�.
		FLOAT fBary1, fBary2;		// barycentric coords of intersection. ������ �����߽� ��ǥ.
		FLOAT fDist;				// distance from ray origin to intersection. ������ �������� ���������� �Ÿ�.
		FLOAT tu, tv;				// texture coords of intersection. ������ �ؽ��� ��ǥ.
	};

private:
	HWND m_hWnd;
	ComPtr<ID3D11Device> m_pDevice = nullptr;

	TVector3 m_vPickRayDir;			// ������ ����
	TVector3 m_vPickRayOrig;		// ������ ����

	DWORD m_dwNumIntersections;		// Number of faces intersected.  ���� ��.

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

