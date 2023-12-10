#pragma once
#include "LStd.h"
#include "LObject.h"

struct L_RAY
{
	float fExtent;
	TVector3 vOrigin;
	TVector3 vDirection;

	L_RAY()
	{
		// fExtent > 0 : ���׸�Ʈ�� ���
		fExtent = -1;
	}
};

class LSelect
{
public:
	L_RAY m_Ray;
	TVector3 m_vIntersection;
	TMatrix m_matWorld;
	TMatrix m_matView;
	TMatrix m_matProj;
public:
	void SetMatrix(TMatrix* pWorld, TMatrix* pView, TMatrix* pProj);
	void Update();
	TVector3 GetIntersection();
	bool GetIntersection(TVector3 vStart, TVector3 vEnd, TVector3 vNormal, TVector3 v0, TVector3 v1, TVector3 v2, float* fPercentage = nullptr);
	bool GetIntersection(float fRayRange, TVector3 vNormal, TVector3 v0, TVector3 v1, TVector3 v2, float* fPercentage = nullptr);
	bool PointInPolygon(TVector3 iVertex, TVector3 faceNormal, TVector3 v0, TVector3 v1, TVector3 v2);
	bool ObjectIsIntersection(LObject* obj);
public:
	LSelect();
	virtual ~LSelect();
};
