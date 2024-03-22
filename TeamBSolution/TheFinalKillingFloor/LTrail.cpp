#include "LTrail.h"

bool LTrail::CreateVertexData()
{
	m_VertexList.resize(12);
	m_VertexList[0] = SimpleVertex(TVector3(0.0f, 0.0f, 0.0f), TVector3(0.0f, 0.0f, 0.0f), TVector4(0.0f, 0.0f, 0.0f, 0.0f), TVector2(0.0f, 0.0f));
	m_VertexList[1] = SimpleVertex(TVector3(0.0f, 0.0f, 0.0f), TVector3(0.0f, 0.0f, 0.0f), TVector4(0.0f, 0.0f, 0.0f, 0.0f), TVector2(0.0f, 1.0f));
	m_VertexList[2] = SimpleVertex(TVector3(0.0f, 0.0f, 0.0f), TVector3(0.0f, 0.0f, 0.0f), TVector4(0.0f, 0.0f, 0.0f, 0.0f), TVector2(1.0f, 0.0f));
	m_VertexList[3] = SimpleVertex(TVector3(0.0f, 0.0f, 0.0f), TVector3(0.0f, 0.0f, 0.0f), TVector4(0.0f, 0.0f, 0.0f, 0.0f), TVector2(1.0f, 1.0f));

	m_VertexList[4] = SimpleVertex(TVector3(0.0f, 0.0f, 0.0f), TVector3(0.0f, 0.0f, 0.0f), TVector4(0.0f, 0.0f, 0.0f, 0.0f), TVector2(0.0f, 0.0f));
	m_VertexList[5] = SimpleVertex(TVector3(0.0f, 0.0f, 0.0f), TVector3(0.0f, 0.0f, 0.0f), TVector4(0.0f, 0.0f, 0.0f, 0.0f), TVector2(0.0f, 1.0f));
	m_VertexList[6] = SimpleVertex(TVector3(0.0f, 0.0f, 0.0f), TVector3(0.0f, 0.0f, 0.0f), TVector4(0.0f, 0.0f, 0.0f, 0.0f), TVector2(1.0f, 0.0f));
	m_VertexList[7] = SimpleVertex(TVector3(0.0f, 0.0f, 0.0f), TVector3(0.0f, 0.0f, 0.0f), TVector4(0.0f, 0.0f, 0.0f, 0.0f), TVector2(1.0f, 1.0f));

	m_VertexList[8] = SimpleVertex(TVector3(0.0f, 0.0f, 0.0f), TVector3(0.0f, 0.0f, 0.0f), TVector4(0.0f, 0.0f, 0.0f, 0.0f), TVector2(0.0f, 0.0f));
	m_VertexList[9] = SimpleVertex(TVector3(0.0f, 0.0f, 0.0f), TVector3(0.0f, 0.0f, 0.0f), TVector4(0.0f, 0.0f, 0.0f, 0.0f), TVector2(0.0f, 1.0f));
	m_VertexList[10] = SimpleVertex(TVector3(0.0f, 0.0f, 0.0f), TVector3(0.0f, 0.0f, 0.0f), TVector4(0.0f, 0.0f, 0.0f, 0.0f), TVector2(1.0f, 0.0f));
	m_VertexList[11] = SimpleVertex(TVector3(0.0f, 0.0f, 0.0f), TVector3(0.0f, 0.0f, 0.0f), TVector4(0.0f, 0.0f, 0.0f, 0.0f), TVector2(1.0f, 1.0f));

	return true;
}

bool LTrail::CreateIndexData()
{
	m_IndexList.resize(30);
	int iIndex = 0;
	m_IndexList[iIndex++] = 3; 	m_IndexList[iIndex++] = 1; 	m_IndexList[iIndex++] = 0; 	m_IndexList[iIndex++] = 2;	m_IndexList[iIndex++] = 3; 	m_IndexList[iIndex++] = 0;
	m_IndexList[iIndex++] = 5; 	m_IndexList[iIndex++] = 3; 	m_IndexList[iIndex++] = 2; 	m_IndexList[iIndex++] = 4;	m_IndexList[iIndex++] = 5; 	m_IndexList[iIndex++] = 2;
	m_IndexList[iIndex++] = 7; 	m_IndexList[iIndex++] = 5; 	m_IndexList[iIndex++] = 4;  m_IndexList[iIndex++] = 6;	m_IndexList[iIndex++] = 7;  m_IndexList[iIndex++] = 4;
	m_IndexList[iIndex++] = 9; 	m_IndexList[iIndex++] = 7; 	m_IndexList[iIndex++] = 6;  m_IndexList[iIndex++] = 8;	m_IndexList[iIndex++] = 9;  m_IndexList[iIndex++] = 6;
	m_IndexList[iIndex++] = 11; m_IndexList[iIndex++] = 9; 	m_IndexList[iIndex++] = 8;  m_IndexList[iIndex++] = 10;	m_IndexList[iIndex++] = 11;  m_IndexList[iIndex++] = 8;
	return true;
}