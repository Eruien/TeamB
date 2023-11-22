#pragma once
#include "LObject.h"
#include <string>

struct LMapDesc
{
	int iNumCols;
	int iNumRows;
	float fCellDistance;
	float fScaleHeight;
	std::wstring TextureFilePath;
	std::wstring ShaderFilePath;
};

class LMap : public LObject
{
public:
	LMapDesc m_MapDesc;
	// 계산하는 요소들
	int m_iNumFace;
	int m_iNumCellCols;
	int m_iNumCellRows;
	// 받아 오는 요소들
	int m_iNumCols;
	int m_iNumRows;
	int m_fCellDistance;
	// height맵 계산
	LVector* m_pFaceNormals;
	int* m_pNormalLookupTable;
	int	 m_iNumVertices;
public:
	bool Load(LMapDesc& mapDesc);
	bool CreateVertexData() override;
	bool CreateIndexData() override;
public:
	virtual bool GenVertexNormal();
	virtual void InitFaceNormals();
	virtual void GenNormalLookupTable();
	virtual void CalcPerVertexNormalsFastLookup();
	virtual void CalcFaceNormals();
	virtual LVector	ComputeFaceNormal(DWORD dwIndex0, DWORD dwIndex1, DWORD dwIndex2);
public:
	virtual float GetHeightOfVertex(int iVertexIndex);
	virtual LVector	GetNormalOfVertex(int iVertexIndex);
	virtual LVector4 GetColorOfVertex(int iVertexIndex);
	virtual LVector2 GetTextureOfVertex(int, int);

};

