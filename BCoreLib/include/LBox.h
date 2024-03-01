#pragma once
#include "LObject.h"

class LBox : public LObject
{
public:
	T_BOX m_Box;
	TMatrix m_BoxWorld;
public:
	void CreateOBBBox(float fExtX = 1.0f, float fExtY = 1.0f, float fExtZ = 1.0f,
		TVector3 vCenter = TVector3(0.0f, 0.0f, 0.0f),
		TVector3 vDirX = TVector3(1.0f, 0.0f, 0.0f),
		TVector3 vDirY = TVector3(0.0f, 1.0f, 0.0f),
		TVector3 vDirZ = TVector3(0.0f, 0.0f, 1.0f));
	bool CreateVertexData() override;
	bool CreateIndexData() override;
	bool CollisionCheckAABB(LBox* other);
	bool CollisionCheckOBB(LBox* other);
	float DotProduct(const TVector3& vec1, const TVector3& vec2);
	bool OverlapOnAxis(const T_BOX& box1, const T_BOX& box2, const TVector3& axis);
	void SetMatrix(TMatrix* matWorld, TMatrix* matView, TMatrix* matProj) override;
};

