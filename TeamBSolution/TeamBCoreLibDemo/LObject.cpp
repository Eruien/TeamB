#include "LObject.h"

void LObject::SetBox(LVector& p, float fw, float fh, float fd)
{
	m_vPosition = p;
	m_Box.Set(p, fw, fh, fd);
}

void LObject::SetBox(LVector& p)
{
	m_vPosition = p;
	m_Box.Set(p, m_Box.m_fWidth, m_Box.m_fHeight, m_Box.m_fDepth);
}

void  LObject::SetMatrix(LMatrix* matWorld, LMatrix* matView, LMatrix* matProj)
{
	if (matWorld != nullptr)
	{
		m_matWorld = *matWorld;
	}

	if (matView != nullptr)
	{
		m_matView = *matView;
	}

	if (matProj != nullptr)
	{
		m_matProj = *matProj;
	}

	m_cbData.matWorld = m_matWorld.Transpose();
	m_cbData.matView = m_matView.Transpose();
	m_cbData.matProj = m_matProj.Transpose();
	m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &m_cbData, 0, 0);
}

void LObject::SetScale(LVector s)
{
	m_vScale = s;
}

void LObject::SetPos(LVector p)
{
	m_vPosition = p;
}

void LObject::SetSprite(float pWidth, float pHeight, float windowWidth, float WindowHeight, float textureSize, int textureIndex)
{

}

void LObject::Move(float dir)
{
	
}

bool LObject::Init()
{	
	LDXObject::Init();
	return true;
}

bool LObject::Frame()
{
	LDXObject::Frame();
	return true;
}

bool LObject::Render()
{
	LDXObject::Render();
	return true;
}

bool LObject::Release()
{
	LDXObject::Release();
	return true;
}

LObject::LObject()
{
	m_vPosition = LVector(0, 0, 0);
	m_vRotation = LVector(0, 0, 0);
	m_vScale = LVector(1, 1, 1);
}

LObject::~LObject() {}
