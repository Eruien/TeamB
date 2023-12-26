#pragma once
#include "LStd.h"
#include "TMath.h"
#include "LInput.h"
#include "LGlobal.h"
#include "LNode.h"
#include "Structs.h"


class Collision
{
public:
	// --------------------------------- Ray ---------------------------------//
	static bool RayToFace(const Ray& ray, const TVector3& v0, const TVector3& v1, const TVector3& v2, TVector3* pickPoint = nullptr);

	// --------------------------------- Cube ---------------------------------//

	static bool CubeToRay(const Cube& cube, const Ray& ray);
	static bool CubeToPlane(const Cube& cube, const Plane& plane);

	// --------------------------------- Circle ---------------------------------//
	static bool CircleToPoint(const Circle& circle, const TVector3& point);
};



enum class CollisionPos
{
	Overlap,
	Behind,
	Front
};
