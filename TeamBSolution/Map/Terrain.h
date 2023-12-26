#pragma once

#include "LStd.h"
#include "HeightMap.h"
#include "LPicking.h"

#include "LPicking.h"
#include "LDXObject.h"
#include "LTimer.h"

class MapQuadTree;

struct TerrainDesc
{
	UINT rowNum = 0;
	UINT colNum = 0;
	float cellDistance = 0;
	float heightScale = 0;

	std::wstring textureFilePath = L"";
	std::wstring heightMapFilePath = L"";
	std::wstring shaderFilePath = L"";

	std::wstring alphaTexPath = L"";

	int DevideTreeDepth = 1;
};

class Terrain : public LDXObject, public enable_shared_from_this<Terrain>
{
private:
	// for ImGUI
	int pickingMode = 0;
	int tillingTextureNum = 0;
	int changeHeightMode = 0;
	float changeHeight = 10.0f;
	float radius = 10.0f;

	
public:
	UINT rowNum;
	UINT colNum;
	UINT rowCellNum;
	UINT colCellNum;
	UINT vertexCount;
	UINT faceCount;
	float cellDistance;
	
	int devideTreeDepth = 1;
	wstring textureFilePath;
	wstring shaderFilePath;

	vector<SimpleVertex> vertices;
	vector<UINT> indices;
	vector<UINT> leafNodeIndexList;
	
	vector<TVector3> faceNormalList;
	vector<int> normalVectorLookTable;
	
	shared_ptr<Shader> shader;
	shared_ptr<Texture> texture;

	shared_ptr<HeightMap> heightMap;
	shared_ptr<LPicking> picking;
	//shared_ptr<Splatting> splatting;
	shared_ptr<MapQuadTree> mapQuadTree;


private:
	bool CreateVertexData() override;
	bool CreateIndexData() override;
	void InitFaceNormal();
	void GenNormalLookupTable();
	void CalcPerVertexNormalsFastLookup();
	void CalcFaceNormals();
	TVector3 ComputeFaceNormal(DWORD dwIndex0, DWORD dwIndex1, DWORD dwIndex2);
	void CreateLeafNodeIndexList();

	// calc function
	void CalcVertexColor(TVector3 vLightDir);
	void UpdateVertexHeight(TVector3 centerPos);

	// temp
	void ObjectSpawn(TVector3 spawnPos);
public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;

	Terrain(TerrainDesc desc);
	~Terrain();
};

