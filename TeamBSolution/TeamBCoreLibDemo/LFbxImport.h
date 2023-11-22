#pragma once
#include "LObject.h"
#include "LFbxObj.h"
#include <fbxsdk.h>
#pragma comment(lib, "libfbxsdk-md.lib")
#pragma comment(lib, "libxml2-md.lib")
#pragma comment(lib, "zlib-md.lib")

class LFbxImport
{
	FbxManager* m_pFbxManager = nullptr;
	FbxImporter* m_pFbxImporter = nullptr;
	FbxScene* m_pFbxScene = nullptr;
	FbxScene* m_pZombieScene = nullptr;
	FbxNode* m_RootNode = nullptr;
	std::vector<FbxNode*> m_MeshNodeList;
public:
	void GetAnimation(LFbxObj* fbxObj);
public:
	FbxVector2 ReadTextureCoord(FbxLayerElementUV* layer, int iVertexIndex, int iIndex);
	FbxColor ReadColor(FbxLayerElementVertexColor* layer, int iVertexIndex, int iIndex);
	FbxVector4 ReadNormal(FbxLayerElementNormal* layer, int iVertexIndex, int iIndex);
	std::string ParseMaterial(FbxSurfaceMaterial* pMaterial);
	int GetSubMaterialIndex(FbxLayerElementMaterial* layer, int iPoly);
public:
	TMatrix ConvertAMatrix(FbxAMatrix& m);
	TMatrix DxConvertMatrix(TMatrix& m);
	TMatrix ParseTransform(FbxNode* fbxNode);
public:
	bool Load(std::wstring fbxFilePath, LFbxObj* fbxObj);
	void PreProcess(FbxNode* fbxNode);
	void MeshLoad(FbxNode* fbxNode, LMesh* lMesh);
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
};