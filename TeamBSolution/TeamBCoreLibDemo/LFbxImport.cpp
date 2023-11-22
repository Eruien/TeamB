#include "LFbxImport.h"

TMatrix LFbxImport::ConvertAMatrix(FbxAMatrix& m)
{
	TMatrix mat;
	float* pMatArray = reinterpret_cast<float*>(&mat);
	double* pSrcArray = reinterpret_cast<double*>(&m);

	for (int i = 0; i < 16; i++)
	{
		pMatArray[i] = pSrcArray[i];
	}

	return mat;
}

TMatrix LFbxImport::DxConvertMatrix(TMatrix& m)
{
	TMatrix mat;
	mat._11 = m._11; mat._12 = m._13; mat._13 = m._12;
	mat._21 = m._31; mat._22 = m._33; mat._23 = m._32;
	mat._31 = m._21; mat._32 = m._23; mat._33 = m._22;
	mat._41 = m._41; mat._42 = m._43; mat._43 = m._42;
	mat._14 = mat._24 = mat._34 = 0.0f; mat._44 = 1.0f;

	return mat;
}

TMatrix LFbxImport::ParseTransform(FbxNode* fbxNode)
{
	FbxTime::EMode TimeMode = FbxTime::GetGlobalTimeMode();
	FbxTime time;
	time.SetFrame(0.0f, TimeMode);
	FbxAMatrix matFbx = fbxNode->EvaluateGlobalTransform(time);
	TMatrix mat = ConvertAMatrix(matFbx);
	mat = DxConvertMatrix(mat);

	return mat;
}

bool LFbxImport::Load(std::wstring fbxFilePath, LFbxObj* fbxObj)
{
	bool ret = m_pFbxImporter->Initialize(wtm(fbxFilePath).c_str());
	m_pFbxImporter->Import(m_pFbxScene);

	if (!ret)
	{
		MessageBoxA(NULL, "FbxFile Import Error", "Error Box", MB_OK);
		return false;
	}

	m_RootNode = m_pFbxScene->GetRootNode();

	if (m_RootNode == nullptr)
	{
		MessageBoxA(NULL, "FbxRootNode is Null", "Error Box", MB_OK);
		return false;
	}
	
	PreProcess(m_RootNode);

	// ���� �̸��� ���� ��θ� �̾Ƴ� path�� ����Ǿ� ����
	std::size_t found = fbxFilePath.find_last_of(L"/");
	std::wstring path = fbxFilePath.substr(0, found + 1);

	for (int i = 0; i < m_MeshNodeList.size(); i++)
	{
		std::shared_ptr<LMesh> lMesh = std::make_shared<LMesh>();
		// �޽��� Texture�迭�� �ε��Ұű� ������ Texture�̸��� �ʿ� ��� ����
		lMesh->m_DefaultFilePath = path;

		// �޽��� �ִ� ��忡�� �޽� ���õ� ������ �̾Ƴ�
		MeshLoad(m_MeshNodeList[i], lMesh.get());
		fbxObj->m_MeshList.push_back(lMesh);
	}

	GetAnimation(fbxObj);

	return true;
}

void LFbxImport::PreProcess(FbxNode* fbxNode)
{
	if (fbxNode == nullptr) return;
	if (fbxNode->GetCamera() || fbxNode->GetLight()) return;

	if (fbxNode->GetMesh())
	{
		m_MeshNodeList.push_back(fbxNode);
	}
	
	for (int i = 0; i < fbxNode->GetChildCount(); i++)
	{
		PreProcess(fbxNode->GetChild(i));
	}
}

void LFbxImport::MeshLoad(FbxNode* fbxNode, LMesh* lMesh)
{
	// �޽��� �ִ� ��忡�� �޽��� ������ ��
	FbxMesh* fbxMesh = fbxNode->GetMesh();
	// �޽��� ������ �ִ� �������� �� ����
	int iPolyCount = fbxMesh->GetPolygonCount();
	// �������� �迭�� ������ ��
	FbxVector4* fbxControlPointList = fbxMesh->GetControlPoints();
	// ���̾���� �ݺ��� ���� ���̾� ī����
	int iLayerCount = fbxMesh->GetLayerCount();
	// �޽��� ������ �ִ� ������ ������ ��ǻ� �̰ʹ�� ������ �����ؼ� �������̱� ������ SubMaterial������� ���� �������.
	int iSubMaterialCount = fbxNode->GetMaterialCount();
	// ������׸��� �ε����� ��� ���� �⺻���� 0���� �ϳ��� ������ ����.
	int iSubMaterialIndex = 0;

	std::vector<FbxLayerElementNormal*> NormalLayerList;
	std::vector<FbxLayerElementVertexColor*> ColorLayerList;
	std::vector<FbxLayerElementUV*> UVLayerList;
	std::vector<FbxLayerElementMaterial*> MaterialLayerList;

	// Mesh�� ������ �ִ� ���̾� ������ ���̾� �迭 �̾Ƴ���
	for (int iLayer = 0; iLayer < iLayerCount; iLayer++)
	{
		FbxLayer* fbxLayer = fbxMesh->GetLayer(iLayer);

		if (fbxLayer->GetNormals() != nullptr)
		{
			NormalLayerList.push_back(fbxLayer->GetNormals());
		}

		if (fbxLayer->GetVertexColors() != nullptr)
		{
			ColorLayerList.push_back(fbxLayer->GetVertexColors());
		}

		if (fbxLayer->GetUVs() != nullptr)
		{
			UVLayerList.push_back(fbxLayer->GetUVs());
		}

		if (fbxLayer->GetMaterials() != nullptr)
		{
			MaterialLayerList.push_back(fbxLayer->GetMaterials());
		}
	}

	// Mesh�� ������ �������� ������ ���� ����Ʈ�� �ִ´�.
	if (iSubMaterialCount > 0)
	{
		lMesh->m_TriangleList.resize(iSubMaterialCount);
	}

	// �ؽ�ó ������ �̸��� �ҷ����� �װ� ������� �ؽ�ó �迭�� �ε��Ѵ�.
	for (int iSub = 0; iSub < iSubMaterialCount; iSub++)
	{
		std::string filePath;
		FbxSurfaceMaterial* surfaceMaterial = fbxNode->GetMaterial(iSub);
		filePath = ParseMaterial(surfaceMaterial);

		char Drive[MAX_PATH];
		char Dir[MAX_PATH];
		char FName[MAX_PATH];
		char Ext[MAX_PATH];

		_splitpath_s(filePath.c_str(), Drive, MAX_PATH, Dir, MAX_PATH, FName, MAX_PATH, Ext, MAX_PATH);

		std::string texName = FName;
		texName += Ext;

		lMesh->m_TexFileNameList.push_back(mtw(texName));
	}

	for (int i = 0; i < lMesh->m_TexFileNameList.size(); i++)
	{
		std::wstring textureName = lMesh->m_DefaultFilePath;
		textureName += lMesh->m_TexFileNameList[i];
		lMesh->LoadTexture(textureName);
	}

	// ��� ������ �ҷ��´�. ��� ������ local���� x �ڽŰ��θ��� ��ȯ��� x �������� ���
	// �ڽŰ� �θ��� ��ȯ����� ParseTransform ���� �������� ����� �ٷ� �Ʒ����� �̾Ƴ���.
	FbxAMatrix GeometircMatrix;
	FbxAMatrix normalMatrix;
	{
		FbxVector4 s = fbxNode->GetGeometricScaling(FbxNode::eSourcePivot);
		FbxVector4 r = fbxNode->GetGeometricRotation(FbxNode::eSourcePivot);
		FbxVector4 t = fbxNode->GetGeometricTranslation(FbxNode::eSourcePivot);

		GeometircMatrix.SetS(s);
		GeometircMatrix.SetR(r);
		GeometircMatrix.SetT(t);

		normalMatrix = GeometircMatrix;
		normalMatrix = normalMatrix.Inverse();
		normalMatrix = normalMatrix.Transpose();
	}

	lMesh->m_MatWorld = ParseTransform(fbxNode);
	
	// ������ ����� �ƴ϶� �������� ���� ��� ����̴�.
	// �ﰢ�� ��� cell�� 9���� ��� 9 x 2 x 3 = 54���� ���� ���
	// �������� ����, �ﰢ���� ����, �ﰢ���� ������ ����
	int iBasePolyIndex = 0;
	for (int iPoly = 0; iPoly < iPolyCount; iPoly++)
	{
		int iPolySize = fbxMesh->GetPolygonSize(iPoly);
		int iTriangleCount = iPolySize - 2;
		// �����￡�� �ش� ������ �ε����� �����´�.
		iSubMaterialIndex = GetSubMaterialIndex(MaterialLayerList[0], iPoly);

		// ������ �� ������ ������ ����
		lMesh->m_NumPoly += iPolySize;
		iBasePolyIndex += iPolySize;
		for (int iTriangle = 0; iTriangle < iTriangleCount; iTriangle++)
		{
			int CornerIndices[3];
			CornerIndices[0] = fbxMesh->GetPolygonVertex(iPoly, 0);
			CornerIndices[1] = fbxMesh->GetPolygonVertex(iPoly, iTriangle + 2);
			CornerIndices[2] = fbxMesh->GetPolygonVertex(iPoly, iTriangle + 1);

			int uv[3];
			uv[0] = fbxMesh->GetTextureUVIndex(iPoly, 0);
			uv[1] = fbxMesh->GetTextureUVIndex(iPoly, iTriangle + 2);
			uv[2] = fbxMesh->GetTextureUVIndex(iPoly, iTriangle + 1);

			int iPolyIndex[3] = { 0, iTriangle + 2, iTriangle + 1 };
		
			for (int iVertex = 0; iVertex < 3; iVertex++)
			{
				SimpleVertex pnct;

				int iDCIndex = CornerIndices[iVertex];
				FbxVector4 v = fbxControlPointList[iDCIndex];
				FbxVector4 finalPos = GeometircMatrix.MultT(v);
				pnct.p.x = finalPos.mData[0];
				pnct.p.y = finalPos.mData[2];
				pnct.p.z = finalPos.mData[1];

				if (NormalLayerList.size() > 0)
				{
					FbxVector4 normal = {0.0f, 0.0f, 0.0f};
					normal = ReadNormal(NormalLayerList[0], iDCIndex, uv[iVertex] + iBasePolyIndex);

					pnct.n.x = normal.mData[0];
					pnct.n.y = normal.mData[2];
					pnct.n.z = normal.mData[1];
				}

				if (ColorLayerList.size() > 0)
				{
					FbxColor color = { 1.0f, 1.0f, 1.0f, 1.0f };
					color = ReadColor(ColorLayerList[0], iDCIndex, uv[iVertex] + iBasePolyIndex);

					pnct.c.x = color.mRed;
					pnct.c.y = color.mGreen;
					pnct.c.z = color.mBlue;
					pnct.c.w = 1.0f;
				}

				FbxVector2 textureUV = ReadTextureCoord(UVLayerList[0], iDCIndex, uv[iVertex]);
				pnct.t.x = textureUV.mData[0];
				pnct.t.y = 1.0f - textureUV.mData[1];

				lMesh->m_TriangleList[iSubMaterialIndex].push_back(pnct);
			}
		}
	}
}

bool LFbxImport::Init()
{
	m_pFbxManager = FbxManager::Create();
	m_pFbxImporter = FbxImporter::Create(m_pFbxManager, IOSROOT);
	m_pFbxScene = FbxScene::Create(m_pFbxManager, "");
	return true;
}

bool LFbxImport::Frame()
{
	return true;
}

bool LFbxImport::Render()
{
	return true;
}

bool LFbxImport::Release()
{
	m_pFbxScene->Destroy();
	m_pFbxImporter->Destroy();
	m_pFbxManager->Destroy();

	m_pFbxScene = nullptr;
	m_pFbxImporter = nullptr;
	m_pFbxManager = nullptr;

	m_MeshNodeList.clear();

	return true;
}