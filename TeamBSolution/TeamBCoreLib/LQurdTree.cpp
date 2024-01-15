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
	m_pCamera = LGlobal::g_pMainCamera;
	
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
	CreateAlphaTexture(LGlobal::g_pDevice.Get(), 1024, 1024);
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

void LQurdtree::UpdateIndexBuffer()
{
	if (m_IndexList.empty()) return;
}

UINT LQurdtree::SelectVertexList(T_BOX tBox, vector<LNode*>& selectNodeList)
{
	for (auto node : m_LeafNodeList)
	{
		if (node != nullptr)
		{
			TCollisionType ret = TCollision::BoxToBox(node->m_tBox, tBox);
			if (ret > 0)
			{
				selectNodeList.push_back(node);
			}
		}
	}
	return selectNodeList.size();
}

HRESULT LQurdtree::CreateAlphaTexture(ID3D11Device* pDevice, DWORD dwWidth, DWORD dwHeight)
{
	HRESULT hr;
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(td));
	td.Width = dwWidth;
	td.Height = dwHeight;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;

	m_fAlphaData = new BYTE[dwWidth * dwHeight * 4];
	for (UINT y = 0; y < dwHeight; y++)
	{
		for (UINT x = 0; x < dwWidth; x++)
		{
			BYTE* pixel = &m_fAlphaData[dwWidth * y * 4 + x * 4];
			pixel[0] = 0;//r
			pixel[1] = 0;//g
			pixel[2] = 0;//b
			pixel[3] = 0;//a
		}
	}
	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = m_fAlphaData;
	initData.SysMemPitch = sizeof(BYTE) * 4 * dwWidth;
	initData.SysMemSlicePitch = 0;
	if (FAILED(hr = pDevice->CreateTexture2D(&td, &initData, m_pMaskAlphaTex.GetAddressOf())))
	{
		return hr;
	}
	if (FAILED(hr = pDevice->CreateShaderResourceView(m_pMaskAlphaTex.Get(), NULL, m_pMaskAlphaTexSRV.GetAddressOf())))
	{
		return hr;
	}

	return hr;
}

void LQurdtree::Splatting(TVector3 vIntersection, UINT iSplattingTexIndex, float fSplattingRadius)
{
	UINT _iSplattingIndex = iSplattingTexIndex;
	UINT const DataSize = sizeof(BYTE) * 4;
	UINT const RowPitch = DataSize * 1024;
	UINT const DepthPitch = 0;
	
	// pick data -> texture data
	// 0 ~ 64 -> 0 ~ 1
	// -32 ~ +32 -> 0 ~ 1024 -> 0 ~ 1
	TVector2 vTexIndex;
	TVector2 vUV;
	TVector2 vMaxSize = { +32, +32 };
	UINT iTexSize = 512;
	TVector3 vTexPos;
	TVector3 vPickPos = vIntersection;
	
	for (UINT y = 0; y < iTexSize; y++)
	{
		vTexIndex.y = y;
		for (UINT x = 0; x < iTexSize; x++)
		{
			vTexIndex.x = x;
			// -1 ~ +1
			vUV = TVector2((vTexIndex.x / (float)iTexSize) * 2.f - 1.f,
							-(vTexIndex.y / (float)iTexSize * 2.f - 1.f));
			// -32 ~ +32
			vTexPos = TVector3(vUV.x * vMaxSize.x, 0.f, vUV.y * vMaxSize.y);
			BYTE* pixel = &m_fAlphaData[iTexSize * y * 4 + x * 4];
			TVector3 t = vPickPos - vTexPos;
			float fRadius = D3DXVec3Length(&t);

			if (fRadius <= fSplattingRadius)
			{
				float fDot = 1.f - (fRadius / fSplattingRadius);
				float fAlpha = fDot * 255.f;
				if (_iSplattingIndex == 0)
					pixel[0] = min(255.f, pixel[0] + fAlpha);
				if (_iSplattingIndex == 1)
					pixel[1] = min(255.f, pixel[1] + fAlpha);
				if (_iSplattingIndex == 2)
					pixel[2] = min(255.f, pixel[2] + fAlpha);
				if (_iSplattingIndex == 3)
					pixel[3] = min(255.f, pixel[3] + fAlpha);
			}
		}
	}
	m_pMap->m_pImmediateContext->UpdateSubresource(m_pMaskAlphaTex.Get(), 0, nullptr, m_fAlphaData, RowPitch, DepthPitch);
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


	m_pImmediateContext->PSSetShaderResources(0, 1, m_pMap->m_Tex->m_pTexSRV.GetAddressOf());

	//m_pImmediateContext->PSSetShaderResources(0, 1, m_pMaskAlphaTex.GetAddressOf());

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
