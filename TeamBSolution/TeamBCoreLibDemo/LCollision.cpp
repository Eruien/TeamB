#include "LCollision.h"

bool LCollision::BoxInPoint(LBox& box, LVector& p)
{
	if (box.m_Min.x <= p.x && box.m_Max.x >= p.x
		&&
		box.m_Min.y <= p.y && box.m_Max.y >= p.y
		&&
		box.m_Min.z <= p.z && box.m_Max.z >= p.z)
	{
		return true;
	}
	return false;
}

bool LCollision::BoxInBox(LBox& box1, LBox& box2)
{
	if (box1.m_Min.x <= box2.m_Min.x && box1.m_Max.x >= box2.m_Max.x
		&&
		box1.m_Min.y <= box2.m_Min.y && box1.m_Max.y >= box2.m_Max.y
		&&
		box1.m_Min.z <= box2.m_Min.z && box1.m_Max.z >= box2.m_Max.z)
	{
		return true;
	}

	return false;
}

CollisionType LCollision::BoxToBox(LBox& box1, LBox& box2)
{
	if (box1 == box2) return CollisionType::CT_OUTSIDE;

	if (BoxInBox(box1, box2))
	{
		return CollisionType::CT_DESTBIG;
	}

	if (BoxInBox(box2, box1))
	{
		return CollisionType::CT_SRCBIG;
	}

	LVector vDis = box1.m_Center - box2.m_Center;
	float dis = vDis.Length();
	LVector vLength = box1.m_Half + box2.m_Half;
	float length = vLength.Length();

	if (dis <= length)
	{
		box2.isCollision = true;
		return CollisionType::CT_SPANNING;
	}

	return CollisionType::CT_OUTSIDE;
}

bool LCollision::CircleToPoint(LCircle circle, LVector& p)
{
	LVector vDis = circle.vCenter - p;
	float dis = vDis.Length();

	if (circle.fRadius <= dis)
	{
		return true;
	}

	return false;
}

bool LCollision::CircleToCircle(LCircle circle1, LCircle circle2)
{
	LVector vDis = circle1.vCenter - circle2.vCenter;
	float dis = vDis.Length();

	if (dis <= (circle1.fRadius + circle2.fRadius))
	{
		return true;
	}

	return false;
}

bool LCollision::SPhereToPoint(LSphere sphere, LVector& p)
{
	LVector vDis = sphere.vCenter - p;
	float dis = vDis.Length();

	if (sphere.fRadius <= dis)
	{
		return true;
	}

	return false;
}

bool LCollision::SPhereToSPhere(LSphere sphere1, LSphere sphere2)
{
	LVector vDis = sphere1.vCenter - sphere2.vCenter;
	float dis = vDis.Length();

	if (dis <= (sphere1.fRadius + sphere2.fRadius))
	{
		return true;
	}

	return false;
}