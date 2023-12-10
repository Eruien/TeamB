#include "LQurdtree.h"
#include "LGlobal.h"
#include "LCamera.h"

void LQurdtree::SetDepth(int depth)
{
	m_TreeDepth = depth;
}

LNode* LQurdtree::NewNode(LNode* pParnet, DWORD topLeft, DWORD topRight, DWORD bottomRight, DWORD bottomLeft)
{
	LNode* newNode = new LNode;
	newNode->SetParent(pParnet);

	newNode->m_Corner.push_back(topLeft);
	newNode->m_Corner.push_back(topRight);
	newNode->m_Corner.push_back(bottomRight);
	newNode->m_Corner.push_back(bottomLeft);

	if (m_TreeDepth == newNode->m_iDepth)
	{
		newNode->m_IsLeaf = true;
	}

	if (newNode->m_IsLeaf)
	{
		
		newNode->m_IndexList.resize((newNode->m_ColCellSize) * (newNode->m_RowCellSize) * 6);
	
		int iCurIndex = 0;
		for (DWORD i = 0; i < newNode->m_RowCellSize; i++)
		{
			for (DWORD j = 0; j < newNode->m_ColCellSize; j++)
			{
				int iNextRow = i + 1;
				int iNextCol = j + 1;
				newNode->m_IndexList[iCurIndex + 0] = i * m_Row + (topLeft + j);
				newNode->m_IndexList[iCurIndex + 1] = i * m_Row + (topLeft + iNextCol);
				newNode->m_IndexList[iCurIndex + 2] = i * m_Row + (topLeft + j) + m_Col;
				newNode->m_IndexList[iCurIndex + 3] = newNode->m_IndexList[iCurIndex + 2];
				newNode->m_IndexList[iCurIndex + 4] = newNode->m_IndexList[iCurIndex + 1];
				newNode->m_IndexList[iCurIndex + 5] = newNode->m_IndexList[iCurIndex + 3] + 1;
				iCurIndex += 6;
			}
		}

		m_LeafIndexList.push_back(newNode->m_IndexList);
	}
	
	ComputeBoundingBox(newNode);
	return newNode;
}

void LQurdtree::BuildTree(LNode* pNode)
{
	if (pNode->m_iDepth > m_TreeDepth - 1)
	{
		return;
	}

	DWORD halfRowCellSize = pNode->m_RowCellSize * 0.5f;
	DWORD halfColCellSize = pNode->m_ColCellSize * 0.5f;

	DWORD TopCenter = pNode->m_Corner[0] + halfColCellSize;
	DWORD LeftCenter = pNode->m_Corner[0] + halfRowCellSize * m_Col;
	DWORD RightCenter = pNode->m_Corner[1] + halfRowCellSize * m_Col;
	DWORD BottomCenter = pNode->m_Corner[3] + halfColCellSize;
	
	DWORD Center = LeftCenter + halfColCellSize;
	
	// 0, 1, 2, 3
	LNode* newNode = NewNode(pNode,
		pNode->m_Corner[0],
		TopCenter,
		Center,
		LeftCenter);
	pNode->m_pChild.push_back(newNode);

	newNode = NewNode(pNode,
		TopCenter,
		pNode->m_Corner[1],
		RightCenter,
		Center);
	pNode->m_pChild.push_back(newNode);

	newNode = NewNode(pNode,
		Center,
		RightCenter,
		pNode->m_Corner[2],
		BottomCenter);
	pNode->m_pChild.push_back(newNode);

	newNode = NewNode(pNode,
		LeftCenter,
		Center,
		BottomCenter,
		pNode->m_Corner[3]);
	pNode->m_pChild.push_back(newNode);

	for (int i = 0; i < pNode->m_pChild.size(); i++)
	{
		BuildTree(pNode->m_pChild[i]);
	}
}

void LQurdtree::BuildQurdTree(LMap* pMap, DWORD row, DWORD col)
{
	m_pMap = pMap;
	m_Row = row;
	m_Col = col;
	m_RowCellSize = m_Row - 1;
	m_ColCellSize = m_Col - 1;
	
	DWORD topLeft = 0;
	DWORD topRight = m_ColCellSize;
	DWORD bottomRight = topRight + m_RowCellSize * m_Col;
	DWORD bottomLeft = topLeft + m_RowCellSize * m_Col;

	m_RootNode = NewNode(nullptr, topLeft, topRight, bottomRight, bottomLeft);
	m_RootNode->m_RowCellSize = m_RowCellSize;
	m_RootNode->m_ColCellSize = m_ColCellSize;

	BuildTree(m_RootNode);
	
	for (int i = 0; i < m_LeafIndexList.size(); i++)
	{
		m_IndexList.insert(m_IndexList.end(), m_LeafIndexList[i].begin(), m_LeafIndexList[i].end());
	}

	LDXObject::CreateIndexBuffer();
}

TVector2 LQurdtree::GetHeightFormNode(LNode* pNode)
{
	DWORD dwTL = pNode->m_Corner[0];
	DWORD dwTR = pNode->m_Corner[1];
	DWORD dwBR = pNode->m_Corner[2];
	DWORD dwBL = pNode->m_Corner[3];

	DWORD dwStartCol = dwTL % m_pMap->m_iNumCols;
	DWORD dwEndCol = dwTR % m_pMap->m_iNumCols;
	DWORD dwStartRow = dwTL / m_pMap->m_iNumCols;
	DWORD dwEndRow = dwBL / m_pMap->m_iNumCols;

	TVector2 vHeight;
	vHeight.x = -99999999.0f;
	vHeight.y = 99999999.0f;

	for (int dwRow = dwStartRow; dwRow < dwEndRow; dwRow++)
	{
		for (int dwCol = dwStartCol; dwCol < dwEndCol; dwCol++)
		{
			DWORD dwCurrent = dwRow * m_pMap->m_iNumCellCols + dwCol;
			if (m_pMap->m_VertexList[dwCurrent].p.y > vHeight.x)
			{
				vHeight.x = m_pMap->m_VertexList[dwCurrent].p.y;
			}
			if (m_pMap->m_VertexList[dwCurrent].p.y < vHeight.y)
			{
				vHeight.y = m_pMap->m_VertexList[dwCurrent].p.y;
			}
		}
	}
	return vHeight;
}

void LQurdtree::ComputeBoundingBox(LNode* pNode)
{
	DWORD dwV0 = pNode->m_Corner[0];
	DWORD dwV1 = pNode->m_Corner[2];
	TVector2 vHeight = GetHeightFormNode(pNode);
	pNode->m_tBox.vMax.x = m_pMap->m_VertexList[dwV1].p.x;
	pNode->m_tBox.vMax.z = m_pMap->m_VertexList[dwV0].p.z;
	pNode->m_tBox.vMin.x = m_pMap->m_VertexList[dwV0].p.x;
	pNode->m_tBox.vMin.z = m_pMap->m_VertexList[dwV1].p.z;
	pNode->m_tBox.vMax.y = vHeight.x;
	pNode->m_tBox.vMin.y = vHeight.y;
	pNode->m_tBox.vAxis[0] = { 1,0,0 };
	pNode->m_tBox.vAxis[1] = { 0,1,0 };
	pNode->m_tBox.vAxis[2] = { 0,0,1 };
	pNode->m_tBox.vCenter = (pNode->m_tBox.vMax + pNode->m_tBox.vMin) * 0.5f;
	pNode->m_tBox.fExtent[0] = pNode->m_tBox.vMax.x - pNode->m_tBox.vCenter.x;
	pNode->m_tBox.fExtent[1] = pNode->m_tBox.vMax.y - pNode->m_tBox.vCenter.y;
	pNode->m_tBox.fExtent[2] = pNode->m_tBox.vMax.z - pNode->m_tBox.vCenter.z;
}

void LQurdtree::FindNode(LNode* pNode)
{

	if (pNode == nullptr) return;
	
	if (CullResult::INSIDE == LGlobal::g_pMainCamera->m_Frustum.CheckOBBInPlane(pNode->m_tBox))
	{
		m_NodeList.push_back(pNode);
		return;
	}

	do
	{
		for (int i = 0; i < pNode->m_pChild.size(); i++)
		{
			if (pNode->m_pChild[i] != nullptr)
			{
				if (CullResult::INSIDE == LGlobal::g_pMainCamera->m_Frustum.CheckOBBInPlane(pNode->m_pChild[i]->m_tBox))
				{
					m_NodeList.push_back(pNode->m_pChild[i]);
					continue;
				}
				m_Queue.push(pNode->m_pChild[i]);
				
			}
		}
		if (m_Queue.empty()) break;
		pNode = m_Queue.front();
		m_Queue.pop();
	} while (pNode);

	return;
}

void LQurdtree::AddLeafNode(LNode* pNode)
{
	if (pNode->m_IsLeaf)
	{
		m_LeafNodeList.push_back(pNode);
		return;
	}

	for (int i = 0; i < pNode->m_pChild.size(); i++)
	{
		AddLeafNode(pNode->m_pChild[i]);
	}
}

//void LQurdtree::UpdateIndexBuffer()
//{
//	if (m_IndexList.empty()) return;
//	D3D11_MAPPED_SUBRESOURCE mappedResource;
//	HRESULT hr = m_pMap->m_pImmediateContext->Map(m_pIndexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
//	if (SUCCEEDED(hr))
//	{
//		memcpy(mappedResource.pData, &m_IndexList.at(0), sizeof(DWORD) * m_IndexList.size());
//		m_pMap->m_pImmediateContext->Unmap(m_pIndexBuffer.Get(), 0);
//	}
//}


 //mouse picking 이후 new 함수. 시진.

void LQurdtree::UpdateIndexBuffer()
{
	if (m_IndexList.empty() || !m_pMap || !m_pMap->m_pDevice || !m_pMap->m_pImmediateContext) return;

	// 새로운 인덱스 버퍼를 생성합니다.
	ComPtr<ID3D11Buffer> tempIndexBuffer;
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(DWORD) * m_IndexList.size();
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = m_IndexList.data();

	HRESULT hr = m_pMap->m_pDevice->CreateBuffer(&bufferDesc, &initData, tempIndexBuffer.GetAddressOf());

	if (FAILED(hr))
	{
		// 실패 처리
		return;
	}

	// 기존의 인덱스 버퍼를 교체합니다.
	m_pIndexBuffer.Swap(tempIndexBuffer);

	// 매핑 및 복사
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = m_pMap->m_pImmediateContext->Map(m_pIndexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (SUCCEEDED(hr))
	{
		memcpy(mappedResource.pData, m_IndexList.data(), sizeof(DWORD) * m_IndexList.size());
		m_pMap->m_pImmediateContext->Unmap(m_pIndexBuffer.Get(), 0);
	}
}


bool LQurdtree::Init()
{
	return true;
}

bool LQurdtree::Frame()
{
	m_NodeList.clear();
	m_LeafNodeList.clear();
	m_IndexList.clear();

	FindNode(m_RootNode);

	for (int i = 0; i < m_NodeList.size(); i++)
	{
		AddLeafNode(m_NodeList[i]);
	}

	for (int i = 0; i < m_LeafNodeList.size(); i++)
	{
		m_IndexList.insert(m_IndexList.end(), m_LeafNodeList[i]->m_IndexList.begin(), m_LeafNodeList[i]->m_IndexList.end());
	}

	UpdateIndexBuffer();

	return true;
}

bool LQurdtree::Render()
{
	m_pMap->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
	m_pMap->PreRender();
	m_pImmediateContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	m_pImmediateContext->DrawIndexed(m_IndexList.size(), 0, 0);
	return true;
}

bool LQurdtree::Release()
{
	return true;
}

LQurdtree::~LQurdtree()
{
	delete m_RootNode;
}



void LQurdtree::PerformMousePicking(LQurdtree* pQuadtree, LHeightMap* m_HeightMap)
{
	// 현재 마우스 위치 가져오기
	POINT ptCursor;
	GetCursorPos(&ptCursor);
	ScreenToClient(LGlobal::g_hWnd, &ptCursor);

	TVector3 v;
	v.x = (((2.0f * ptCursor.x) / LGlobal::g_WindowWidth) - 1) / m_matProj._11;
	v.y = -(((2.0f * ptCursor.y) / LGlobal::g_WindowHeight) - 1) / m_matProj._22;
	v.z = 1.0f;

	TMatrix mWorldView = m_matView;
	TMatrix m;
	D3DXMatrixInverse(&m, NULL, &mWorldView);

	LRay pickRay;
	pickRay.m_Origin = TVector3(0.0f, 0.0f, 0.0f);
	pickRay.m_Direction = TVector3(v.x, v.y, v.z);
	D3DXVec3TransformCoord(&pickRay.m_Origin, &pickRay.m_Origin, &m);
	D3DXVec3TransformNormal(&pickRay.m_Direction, &pickRay.m_Direction, &m);
	D3DXVec3Normalize(&pickRay.m_Direction, &pickRay.m_Direction);

	// Quadtree와 교차하는 노드 찾기
	LNode* pSelectedNode = nullptr;
	pQuadtree->FindIntersectedNode(pickRay, pSelectedNode);

	// 선택된 노드가 있으면 해당 노드의 높이를 변경
	if (pSelectedNode)
	{

		// 선택된 노드의 인덱스 리스트 업데이트
		pSelectedNode->UpdateIndexList(m_HeightMap);

	}
	// 인덱스 버퍼 업데이트
	pQuadtree->UpdateIndexBuffer();
}


void LQurdtree::FindIntersectedNode(LRay& ray, LNode*& pSelectedNode)
{
	m_Queue.push(m_RootNode);

	while (!m_Queue.empty())
	{
		LNode* pNode = m_Queue.front();
		m_Queue.pop();

		if (pNode->IntersectRay(ray))
		{
			if (pNode->m_IsLeaf)
			{
				pSelectedNode = pNode;
				return;
			}
			else
			{
				for (int i = 0; i < pNode->m_pChild.size(); i++)
				{
					if (pNode->m_pChild[i] != nullptr)
					{
						m_Queue.push(pNode->m_pChild[i]);
					}
				}
			}
		}
	}

	// 더 이상 리프 노드를 찾을 수 없을 때
	pSelectedNode = nullptr;
}