#pragma once
#include "LCamera.h"
#include "LInput.h"
#include "LGlobal.h"
#include "LHeightMap.h"
#include "LWrite.h"

struct Ray
{
	TVector3 origin;
	TVector3 direction;
};

class LPick
{
public:
	// 
	Ray _ray;
	TVector3 _vIntersection;
	
	void CalcRay();
	bool GetIntersection(float fRange, TVector3 vNormal, TVector3 v0, TVector3 v1, TVector3 v2);
	bool MapIsIntersection(LHeightMap* map);
	bool PointInPolygon(TVector3 vertex, TVector3 faceNormal, TVector3 v0, TVector3 v1, TVector3 v2);
	
public:
	//
	


	LPick();
	virtual ~LPick();

	static LPick& GetInstance()
	{
		static LPick pick;
		return pick;
	}
};

