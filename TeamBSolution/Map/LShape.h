#pragma once
#include "LDXObject.h"
#include "LCore.h"
#include "LStd.h"


struct PC_VERTEX
{
	TVector3 p;
	TVector4 c;
	bool operator == (const PC_VERTEX& Vertex)
	{
		if (p == Vertex.p && c == Vertex.c)
		{
			return true;
		}
		return  false;
	}
	PC_VERTEX() {}
	PC_VERTEX(TVector3		vp,
		TVector4		vc)
	{
		p = vp, c = vc;
	}
};



class LShape : public LDXObject
{

public:
	//--------------------------------------------------------------------------------------
	// Collision Data
	//--------------------------------------------------------------------------------------
	T_BOX                       m_Box;
	T_BOX						m_InitBox;
	T_SPHERE                    m_Sphere;
	T_PLANE                     m_Plane;
	float						m_fTickSpeed;
public:
	TVector3 m_vCenter;
	TVector3 m_vPrevCenter;
	TVector3 m_vMove;
	TVector3 m_vLook; // Z
	TVector3 m_vUp; // Y
	TVector3 m_vSide; // X
	bool m_bUpdateCollision;

public:
	void	CreateOBBBox(float fExtX = 1.0f, float fExtY = 1.0f, float fExtZ = 1.0f,
		TVector3 vCenter = TVector3(0.0f, 0.0f, 0.0f),
		TVector3 vDirX = TVector3(1.0f, 0.0f, 0.0f),
		TVector3 vDirY = TVector3(0.0f, 1.0f, 0.0f),
		TVector3 vDirZ = TVector3(0.0f, 0.0f, 1.0f));
	void	CreateAABBBox(TVector3 vMax = TVector3(1.0f, 1.0f, 1.0f),
		TVector3 vMin = TVector3(-1.0f, -1.0f, -1.0f));
public:
	LShape() {};
	virtual ~LShape(void) {};
};
class LLineShape : public LShape
{
public:
	vector<PC_VERTEX>	m_LineVertexList;
public:
	HRESULT				SetInputLayout();
	bool				CreateVertexData();
	bool				CreateIndexData();
	HRESULT				CreateVertexBuffer();
	HRESULT				CreateResource();
	bool				Draw(ID3D11DeviceContext* pContext, TVector3 vStart, TVector3 vEnd, TVector4 dwColor);
public:
	LLineShape(void);
	virtual ~LLineShape(void);
};

class LDirectionLineShape : public LShape
{
public:
	vector<PC_VERTEX>	m_LineVertexList;
public:
	HRESULT			SetInputLayout();
	bool			CreateVertexData();
	bool			CreateIndexData();
	HRESULT			CreateVertexBuffer();
	HRESULT			CreateResource();
public:
	LDirectionLineShape(void);
	virtual ~LDirectionLineShape(void);
};

class LBoxShape : public LShape
{
public:
	bool			CreateVertexData();
	bool			CreateIndexData();
	HRESULT			CreateResource();
public:
	LBoxShape(void);
	virtual ~LBoxShape(void);
};

class LPlaneShape : public LShape
{
public:
	bool			CreateVertexData();
	bool			CreateIndexData();
	HRESULT			CreateResource();
	bool SetScreenVertex(float x,
		float y,
		float w,
		float h,
		TVector2 vScreen);
public:
	LPlaneShape(void);
	virtual ~LPlaneShape(void);
};
class LSphereShape : public LShape
{
public:
	bool			CreateVertexData();
	bool			CreateIndexData();
	bool			Render(ID3D11DeviceContext* pImmediateContext);
	HRESULT			CreateResource();
	void CreateSphere(UINT Slices = 20, float fRadius = 1.0f);
	void MakeSpherePoint(std::vector<TVector3>& spherePoints,
		UINT Slices = 20, double r = 1.0f,
		TVector3 center = TVector3(0, 0, 0));
public:
	LSphereShape(void);
	virtual ~LSphereShape(void);
};