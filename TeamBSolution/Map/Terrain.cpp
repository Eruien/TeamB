#include "pch.h"
#include "Terrain.h"
#include "MapQuadTree.h"

bool Terrain::CreateVertexData()
{
	vertices.resize(vertexCount);

	float fOffsetU = 1.f / (colNum - 1);
	float fOffsetV = 1.f / (rowNum - 1);
	float fHalfCols = (colCellNum) * 0.5f;
	float fHalfRows = (rowCellNum) * 0.5f;

	float tY;
	for (int iRow = 0; iRow < rowNum; iRow++)
	{
		for (int iCol = 0; iCol < colNum; iCol++)
		{
			int iVertexIndex = iRow * colNum + iCol;
			
			vertices[iVertexIndex].p.x = (iCol - fHalfCols) * cellDistance;
			vertices[iVertexIndex].p.z = -((iRow - fHalfRows) * cellDistance);

			tY = heightMap->GetHeightByIndex(iVertexIndex);
			vertices[iVertexIndex].p.y = tY;

			vertices[iVertexIndex].n = TVector3(0, 1, 0); // 법선벡터
			vertices[iVertexIndex].c = TVector4(0, 0, 0, 0);
			vertices[iVertexIndex].t = TVector2(iCol * fOffsetU, iRow * fOffsetV);
		}
	}
	return true;
}

bool Terrain::CreateIndexData()
{
	indices.resize(faceCount * 3);
	int iIndex = 0;
	for (int iRow = 0; iRow < rowCellNum; iRow++)
	{
		for (int iCol = 0; iCol < colCellNum; iCol++)
		{
			int nextCol = iCol + 1;
			int nextRow = iRow + 1;

			indices[iIndex + 0] = iRow * colNum + iCol;
			indices[iIndex + 1] = iRow * colNum + nextCol;
			indices[iIndex + 2] = nextRow * colNum + iCol;

			indices[iIndex + 3] = indices[iIndex + 2];
			indices[iIndex + 4] = indices[iIndex + 1];
			indices[iIndex + 5] = nextRow * colNum + nextCol;

			iIndex += 6;
		}
	}

	return true;
}

void Terrain::InitFaceNormal()
{
	faceNormalList.resize(faceCount);
	for (int i = 0; i < faceCount; i++)
	{
		faceNormalList[i] = TVector3(0, 0, 0);
	}
}

void Terrain::GenNormalLookupTable()
{
	if (normalVectorLookTable.size() > 0)
	{
		normalVectorLookTable.clear();
		normalVectorLookTable.shrink_to_fit();
	}
	UINT buffersize = rowNum * colNum * 6;
	normalVectorLookTable.resize(buffersize);

	for (int i = 0; i < buffersize; i++)
	{
		normalVectorLookTable[i] = -1;
	}

	for (int i = 0; i < faceCount; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 6; k++)
			{
				UINT vertex = indices[i * 3 + j];					// i 와 j의 함수이므로 k for문 위에 있어도 괜찮을 듯?
				if (normalVectorLookTable[vertex * 6 + k] == -1)
				{
					normalVectorLookTable[vertex * 6 + k] = i;
					break;
				}
			}
		}
	}
}

void Terrain::CalcPerVertexNormalsFastLookup()
{
	CalcFaceNormals();

	int j = 0;
	for (int i = 0; i < vertexCount; i++)
	{
		TVector3 avgNormal;
		avgNormal = TVector3(0.f, 0.f, 0.f);

		for (j = 0; j < 6; j++)
		{
			int triIndex;
			triIndex = normalVectorLookTable[i * 6 + j];

			if (triIndex != -1)
				avgNormal += faceNormalList[triIndex];
			else
				break;
		}

		_ASSERT(j > 0);
		avgNormal.x /= (float)j;
		avgNormal.y /= (float)j;
		avgNormal.z /= (float)j;
		avgNormal.Normalize();

		vertices[i].n = avgNormal;
	}

	CalcVertexColor(TVector3(0, -1, 0));
}

void Terrain::CalcFaceNormals()
{
	int j = 0;
	for (int i = 0; i < faceCount; i += 3)
	{
		DWORD i0 = indices[i];
		DWORD i1 = indices[i + 1];
		DWORD i2 = indices[i + 2];
		faceNormalList[j] = ComputeFaceNormal(i0, i1, i2);
		j++;
	}
}

TVector3 Terrain::ComputeFaceNormal(DWORD dwIndex0, DWORD dwIndex1, DWORD dwIndex2)
{
	TVector3 vNormal;
	TVector3 v0 = vertices[dwIndex1].p - vertices[dwIndex0].p;
	TVector3 v1 = vertices[dwIndex2].p - vertices[dwIndex0].p;

	v0.Cross(v1, vNormal);
	vNormal.Normalize();

	return vNormal;
}

void Terrain::CreateLeafNodeIndexList()
{
	UINT leafRowCellNum = rowNum / pow(2, devideTreeDepth); // pow : 거듭제곱 2^devideTreeDepth

	leafNodeIndexList.resize(leafRowCellNum * leafRowCellNum * 6);

	UINT colNum = leafRowCellNum + 1;

	UINT iIndex = 0;
	for (int iRow = 0; iRow < leafRowCellNum; ++iRow)
	{
		for (int iCol = 0; iCol < leafRowCellNum; ++iCol)
		{
			int nextCol = iCol + 1;
			int nextRow = iRow + 1;

			leafNodeIndexList[iIndex + 0] = iRow * colNum + iCol;
			leafNodeIndexList[iIndex + 1] = iRow * colNum + nextCol;
			leafNodeIndexList[iIndex + 2] = nextRow * colNum + iCol;

			leafNodeIndexList[iIndex + 3] = leafNodeIndexList[iIndex + 2];
			leafNodeIndexList[iIndex + 4] = leafNodeIndexList[iIndex + 1];
			leafNodeIndexList[iIndex + 5] = nextRow * colNum + nextCol;

			iIndex += 6;
		}
	}

}

void Terrain::CalcVertexColor(TVector3 vLightDir)
{
	for (int iRow = 0; iRow < rowNum; iRow++)
	{
		for (int iCol = 0; iCol < colNum; iCol++)
		{

			int  iVertexIndex = iRow * colNum + iCol;

			float fDot = vLightDir.Dot(vertices[iVertexIndex].n);

			vertices[iVertexIndex].c *= fDot;
			vertices[iVertexIndex].c.w = 0.0f;
		}
	}
}

void Terrain::UpdateVertexHeight(TVector3 centerPos)
{
	float distance = 0.f;
	float deltaTime = LGlobal::g_fSPF;

	TVector2 center = TVector2(centerPos.x, centerPos.z);
	for (UINT i : picking->UpdateVertexIndexList)
	{
		distance = (TVector2(vertices[i].p.x, vertices[i].p.z) - center).Length();
		distance = (distance / radius);
		distance = -(distance - 1);

		switch (changeHeightMode)
		{
		case 0:
			vertices[i].p.y += (distance * changeHeight * deltaTime);
			break;
		case 1:
			vertices[i].p.y += (changeHeight * deltaTime);
			break;
		default:
			break;
		}

		heightMap->heightList[i] = vertices[i].p.y;
	}

}

void Terrain::ObjectSpawn(TVector3 spawnPos)
{
	shared_ptr<LDXObject> obj = make_shared<LDXObject>();

	// 추가 필요
}

bool Terrain::Init()
{
	CreateVertexData();
	CreateIndexData();

	// GenerateVertexNormal, make normal table;
	InitFaceNormal();
	GenNormalLookupTable();
	CalcPerVertexNormalsFastLookup();

	// quad tree
	mapQuadTree = std::make_shared<MapQuadTree>(shared_from_this());
	mapQuadTree->maxDepth = devideTreeDepth;
	mapQuadTree->Init();

	// init color
	//splatting->SetVertexByTexture(vertices);

	// create leaf node index list(for picking)
	CreateLeafNodeIndexList();


	// temp (object spawn)
	/*{
		objectShader = std::make_shared<Shader>(L"23. RenderDemo.fx");

		RenderManager::GetInstance().Init(objectShader);
	}*/
	return true;
}

bool Terrain::Frame()
{
	/*ImGui::InputInt("Change Mode", &changeHeightMode);
	ImGui::InputFloat("Change Height", &changeHeight);
	ImGui::InputFloat("Radius", &radius);

	ImGui::InputInt("Picking Mode", &pickingMode);
	ImGui::InputInt("Tilling Texture", &tillingTextureNum);*/

	// for picking
	{
		if (LInput::GetInstance().m_MouseState[0] > KEY_UP)
		{
			auto& ray = LGlobal::g_pMainCamera->ray;
			TVector3 pickPoint;

			bool isFInd = false;
			shared_ptr<MapNode>& pickNode = picking->FindPickedFace(ray, leafNodeIndexList, mapQuadTree->leafNodeMap, pickPoint);

			if (pickNode)
			{
				picking->FindVertex(pickPoint, radius, mapQuadTree->leafNodeMap.size(),
					pickNode, vertices);

				switch (pickingMode)
				{
				case(0):
					UpdateVertexHeight(pickPoint);
					break;
				case(1):
					//splatting->TillingTexture(pickPoint, tillingTextureNum, vertices, picking->UpdateVertexIndexList);
					break;
				case(2):
					if (LInput::GetInstance().m_MouseState[0] == KEY_PUSH)
						ObjectSpawn(pickPoint);
					break;
				default:
					break;
				}
				// update vertex buffer
				mapQuadTree->UpdateVertex(picking->UpdateNodeIndexList);
			}
		}

		//save
		if (LInput::GetInstance().GetKey(DIK_M) == KEY_PUSH)
		{
			heightMap->SaveHeightMap(L"../../res/heightmap/Terrain/heightMap513.bmp");
			//splatting->SaveAlphaTexture(L"../../Res/Textures/Terrain/heightExported.PNG");
		}
	}

	mapQuadTree->Update();
	//splatting->SetSRV(shader);

	return true;
}

bool Terrain::Render()
{
	shader->GetSRV("MapBaseTexture")->SetResource(texture->GetShaderResourceView().Get());
	return true;
}

Terrain::Terrain(TerrainDesc desc)
{
	if (desc.textureFilePath.length() < 1 || desc.shaderFilePath.length() < 1)
		assert(false);

	devideTreeDepth = desc.DevideTreeDepth;
	cellDistance = desc.cellDistance;

	//shader texture
	shader = make_shared<Shader>(desc.shaderFilePath);
	texture = make_shared<Texture>();
	texture->Load(desc.textureFilePath);
	picking = make_shared<LPicking>();
	
	// renderManager가 없으니, 구현해야함.


	heightMap = make_shared<HeightMap>();
	rowNum = desc.rowNum;
	colNum = desc.colNum;
	heightMap->Init(rowNum, colNum, desc.heightScale, desc.heightMapFilePath);

	rowCellNum = rowNum - 1;
	colCellNum = colNum - 1;
	vertexCount = rowNum * colNum;
	faceCount = rowCellNum * colCellNum * 2;

	// splatting



}

Terrain::~Terrain()
{
}
