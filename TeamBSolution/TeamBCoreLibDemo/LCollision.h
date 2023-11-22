#pragma once
#include "LUtils.h"

enum class CollisionType
{
	CT_OUTSIDE = 0,
	CT_SPANNING = 1,
	CT_DESTBIG,
	CT_SRCBIG,
};

class LCollision
{
public:
	static bool BoxInPoint(LBox& box, LVector& p);

	static bool BoxInBox(LBox& box1, LBox& box2);

	static CollisionType BoxToBox(LBox& box1, LBox& box2);

	static bool CircleToPoint(LCircle circle, LVector& p);

	static bool CircleToCircle(LCircle circle1, LCircle circle2);

	static bool SPhereToPoint(LSphere sphere, LVector& p);

	static bool SPhereToSPhere(LSphere sphere1, LSphere sphere2);
};

