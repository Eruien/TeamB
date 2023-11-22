#pragma once
#include"LDXObject.h"

class LObject : public LDXObject
{
public:
	int m_iCollisionID = -1;
	int m_iSelectID = -1;

	virtual void MouseOverlap(LVector mousePos, DWORD state) {}
public:
	LBox m_Box;
	LMatrix m_matWorld;
	LMatrix m_matView;
	LMatrix m_matProj;
	LVector m_vScale;
	LVector m_vRotation;
	LVector m_vPosition;
	virtual void SetBox(LVector& p, float fw, float fh, float fd);
	virtual void SetBox(LVector& p);
	void  SetMatrix(LMatrix* matWorld, LMatrix* matView, LMatrix* matProj);
	void  SetScale(LVector s);
	void  SetPos(LVector p);
	virtual void SetSprite(float pWidth, float pHeight, float windowWidth, float WindowHeight, float textureSize, int textureIndex);
	virtual void Move(float dir);
public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual bool Release() override;
public:
	LObject();
	virtual ~LObject();
};

