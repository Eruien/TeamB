#include "Terrain.h"
#include "LGlobal.h"

bool Terrain::Init()
{
	// light
	m_cbLight.g_cAmbientMaterial = TVector4(0.3f, 0.3f, 0.3f, 1);
	m_cbLight.g_cDiffuseMaterial = TVector4(1, 1, 1, 1);
	m_cbLight.g_cAmbientLightColor = TVector4(1, 1, 1, 1);
	m_cbLight.g_cDiffuseLightColor = TVector4(1, 1, 1, 1);

	// create constantbuffer

	m_pConstantBufferLight.Attach(CreateConstantBuffer(
		m_pDevice.Get(), &m_cbLight, 1, sizeof(LIGHT_CONSTANT_BUFFER)));



	m_DebugCamera = std::make_shared<CamForTool>();
	m_DebugCamera->m_fCameraPitch = 89.6f;
	m_DebugCamera->CreateLookAt({ 0.0f, 1000.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0, 0, 1 });
	m_DebugCamera->CreatePerspectiveFov(L_PI * 0.25, (float)LGlobal::g_WindowWidth / (float)LGlobal::g_WindowHeight, 1.0f, 10000.0f); // SetProjMatrix
	LGlobal::g_pMainCamera = m_DebugCamera.get();

	m_HeightMap = new LHeightMap;
	m_HeightMap->Set();
	m_HeightMap->CreateHeightMap(L"../../res/Heightmap/heightMap513.bmp");

	// 텍스처랑 상관없음 맵 크기를 정하면 그 크기만큼 맵이 만들어진다.
	// 크기는 2의n승 +1 크기만 가능하다.
	// ex 513, 1025,
	LMapDesc MapDesc = {};
	MapDesc.iNumCols = m_HeightMap->m_iNumCols;
	MapDesc.iNumRows = m_HeightMap->m_iNumRows;
	MapDesc.fCellDistance = 1.0f;
	MapDesc.fScaleHeight = 1.0f;
	
	MapDesc.ShaderFilePath = L"../../res/hlsl/DiffuseLight.hlsl";
	MapDesc.TextureFilePath = L"../../res/map/basecolor.jpg";
	m_HeightMap->Load(MapDesc);

	// 프러스텀 컬링용 트리 
	m_Tree = new LQurdtree;
	m_Tree->Set();
	m_Tree->m_TreeDepth = 3;
	// 프러스텀 컬링할 맵이랑 맵의 크기를 입력
	m_Tree->BuildQurdTree(m_HeightMap, 513, 513);



	m_Select = std::make_shared<LSelect>();
	return true;
}

bool Terrain::Frame()
{
	//m_pDefaultCamera->Frame();
	m_vLightVector.x = 1.f;
	m_vLightVector.y = 1.f;
	m_vLightVector.z = 1.f;
	
	D3DXVec3Normalize(&m_vLightVector, &m_vLightVector);
	m_vLightVector *= -1.f;

	m_Select->SetMatrix(nullptr,
		&m_DebugCamera->m_matView, &m_DebugCamera->m_matProj);


	m_Tree->Frame();

	if (m_bUpPicking || m_bDownPicking)
	{
		if (GetIntersection())
		{
			vector<LNode*> nodelist;
			T_BOX box;
			TVector3 vMin, vMax;
			TVector3 vRange(100, 100, 100);
			vMax = m_Select->m_vIntersection + vRange;
			vMin = m_Select->m_vIntersection - vRange;
			box.Set(vMax, vMin);
			if (m_Tree->SelectVertexList(box, nodelist) > 0)
			{
				float fWorkRadius = 20.f; // randstep(3.0f, 20.0f); // 무작위값
				for (auto node : nodelist)
				{
					for (UINT iVertex = 0; iVertex < m_Tree->m_pMap->m_VertexList.size(); iVertex++)
					{
						TVector3 v0 = m_Tree->m_pMap->m_VertexList[iVertex].p;
						TVector3 v = v0 - m_Select->m_vIntersection;
						float fDistance = D3DXVec3Length(&v);
						if (fDistance <= (fWorkRadius + 3.f))
						{
							if (fDistance < fWorkRadius)
							{
								float fValue = (fDistance / fWorkRadius) * 90.f;
								float fDot = cosf(DegreeToRadian(fValue));
								if (m_bUpPicking)
									m_Tree->m_pMap->m_VertexList[iVertex].p.y += fDot * 100; // += fDot *LGlobal::g_fSPF;
								if (m_bDownPicking)
									m_Tree->m_pMap->m_VertexList[iVertex].p.y -= fDot * 100;    // -= fDot *LGlobal::g_fSPF;

								if (node->m_tBox.vMin.y > m_Tree->m_pMap->m_VertexList[iVertex].p.y)
								{
									node->m_tBox.vMin.y = m_Tree->m_pMap->m_VertexList[iVertex].p.y;
								}
								if (node->m_tBox.vMax.y > m_Tree->m_pMap->m_VertexList[iVertex].p.y)
								{
									node->m_tBox.vMax.y = m_Tree->m_pMap->m_VertexList[iVertex].p.y;
								}
							}
							//
						}
					}
					node->m_tBox.vCenter = (node->m_tBox.vMax + node->m_tBox.vMin) * 0.5f;
					node->m_tBox.vAxis[0] = { 1,0,0 };
					node->m_tBox.vAxis[1] = { 0,1,0 };
					node->m_tBox.vAxis[2] = { 0,0,1 };
					node->m_tBox.fExtent[0] = node->m_tBox.vMax.x - node->m_tBox.vCenter.x;
					node->m_tBox.fExtent[1] = node->m_tBox.vMax.y - node->m_tBox.vCenter.y;
					node->m_tBox.fExtent[2] = node->m_tBox.vMax.z - node->m_tBox.vCenter.z;
				}
				m_Tree->m_pMap->UpdateVertexBuffer();
				
			}
			
			// 오브젝트 position update
			/*for (auto npc : m_WorldObjectList)
			{
				TVector3 vPos = npc->m_vPos;
				vPos.y = npc->m_matWorld._42 =
					m_Quadtree.m_pMap->GetHeight(vPos.x, vPos.z);
				npc->SetPos(vPos);
			}*/
		}
		//m_Tree->m_pMap->CalcPerVertexNormalsFastLookup();
	}

	if (m_bObjectPicking)
	{
		if (GetIntersection())
		{
			/*if (m_pTitle && m_pTitle->m_pMap)
			{
				LoadFbx(m_szSelectFbxFile, m_Select.m_vIntersection);
			};*/
		}
	}

	static float fStep = 0.f;
	fStep += LGlobal::g_fSPF;
	if (m_bSplatting && fStep > 0.1f)
	{
		if (GetIntersection())
		{
			fStep = 0.f;
			m_Tree->Splatting(m_Select->m_vIntersection, m_iSplattingTexIndex);
		}
	}
	return true;
}

bool Terrain::Render()
{
	m_pImmediateContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), 0);
	//m_pImmediateContext->OMSetBlendState(m_AlphaBlend.Get(), 0, 0xffffffff);

	TMatrix matWorld, matScale;
	D3DXMatrixScaling(&matScale, 100, 100, 100);
	//D3DXMatrixRotationY(&matWorld, LGlobal::g_fGameTimer);
	D3DXMatrixMultiply(&matWorld, &matScale, &matWorld);
	matWorld._42 = 200.0f;

	m_cbLight.g_vLightDir.x = m_vLightVector.x;
	m_cbLight.g_vLightDir.y = m_vLightVector.y;
	m_cbLight.g_vLightDir.z = m_vLightVector.z;
	m_cbLight.g_vLightDir.w = 1;
	TMatrix matInvWorld;
	D3DXMatrixInverse(&matInvWorld, NULL, &matWorld);
	D3DXMatrixTranspose(&matInvWorld, &matInvWorld);
	D3DXMatrixTranspose(&m_cbLight.g_matInvWorld, &matInvWorld);


	
	m_pImmediateContext->UpdateSubresource(m_pConstantBufferLight.Get(), 0, NULL, &m_cbLight, 0, 0);
	m_pImmediateContext->VSSetConstantBuffers(1, 1, m_pConstantBufferLight.GetAddressOf());
	m_pImmediateContext->PSSetConstantBuffers(1, 1, m_pConstantBufferLight.GetAddressOf());
	//
	// sphere render needed
	//	

	D3DXMatrixIdentity(&m_cbLight.g_matInvWorld);
	m_pImmediateContext->UpdateSubresource(m_pConstantBufferLight.Get(), 0, NULL, &m_cbLight, 0, 0);
	//m_HeightMap->SetMatrix(&m_DebugCamera->m_matWorld, &m_DebugCamera->m_matView, &m_DebugCamera->m_matProj);
	//m_HeightMap->Render();


	m_Tree->Render();

	return true;
}

bool Terrain::Release()
{
	
	return true;
}

ID3D11Buffer* Terrain::CreateConstantBuffer(ID3D11Device* pd3dDevice, void* data, UINT iNumIndex, UINT iSize, bool bDynamic)
{
	HRESULT hr = S_OK;
	ID3D11Buffer* pBuffer = nullptr;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	if (bDynamic)
	{
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0;
	}
	bd.ByteWidth = iSize * iNumIndex;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	InitData.pSysMem = data;
	if (data != NULL)
	{
		if (FAILED(hr = pd3dDevice->CreateBuffer(&bd, &InitData, &pBuffer)))
		{
			//H(hr);
			return nullptr;
		}
	}
	else
	{
		if (FAILED(hr = pd3dDevice->CreateBuffer(&bd, NULL, &pBuffer)))
		{
			//H(hr);
			return nullptr;
		}
	}
	return pBuffer;
}

bool Terrain::GetIntersection()
{
	
	if (LInput::GetInstance().m_MouseState[0] == KEY_PUSH || LInput::GetInstance().m_MouseState[0] == KEY_HOLD)
	{
		for (auto node : m_Tree->m_LeafNodeList)
		{
			UINT index = 0;
			UINT iNumFace = node->m_IndexList.size() / 3;
			for (UINT face = 0; face < iNumFace; face++)
			{
				UINT i0 = node->m_IndexList[index + 0];
				UINT i1 = node->m_IndexList[index + 1];
				UINT i2 = node->m_IndexList[index + 2];
				TVector3 v0 = m_Tree->m_pMap->m_VertexList[i0].p;
				TVector3 v1 = m_Tree->m_pMap->m_VertexList[i1].p;
				TVector3 v2 = m_Tree->m_pMap->m_VertexList[i2].p;
				if (m_Select->ChkPick(v0, v1, v2))
					return true;
				index += 3;
			}
		}
	}
	return false;
}

Terrain::~Terrain() {}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR IpCmdLine, int nCmdShow)
{
	Terrain win;
	win.SetRegisterWindowClass(hInstance);
	win.SetCreateWindow(L"TeamBProject", 800, 600);
	win.Run();
}


