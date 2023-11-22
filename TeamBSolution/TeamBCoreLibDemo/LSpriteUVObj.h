#pragma once
#include "LPlaneObj.h"

class LSpriteUVObj : public LPlaneObj
{
public:
	LVector m_Target;
	LVector m_fDirection;
	bool m_IsDirectionCal = false;
public:
	std::vector<LRectUV> m_UVList;
public:
	float m_OffsetTime = 0.0f;
	float m_AnimationElapsed = 0.0f;
	int m_AnimationIndex = 0;
public:
	void SetTarget(LVector& t);
	void SetUVAnimation(int spriteCount, float animationTime);
	void SetUVPosition();
	void SetUVPositionReverse();
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	LSpriteUVObj();
};

