#include "LModel.h"
#include "LGlobal.h"
#include "LFbxMgr.h"

std::vector<LBoneWorld> LModel::m_texBoneArray;
int LModel::m_AllAnimationFrame = 0;

void LModel::SetAnimationArrayTexture()
{
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width = m_matBoneArraySize * 4;
	// ��ü �����Ӽ�
	texDesc.Height = m_AllAnimationFrame;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.SampleDesc.Count = 1;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;
	
	D3D11_SUBRESOURCE_DATA initialData;
	ZeroMemory(&initialData, sizeof(initialData));
	initialData.pSysMem = &m_texBoneArray.at(0);
	initialData.SysMemPitch = m_matBoneArraySize * 4 * 16;
	initialData.SysMemSlicePitch = 0;

	HRESULT hr = LGlobal::g_pDevice->CreateTexture2D(&texDesc, &initialData, pAnimationArrayTex.GetAddressOf());

	if (FAILED(hr))
	{
		MessageBoxA(NULL, "Create AnimationArrayTex 2DTexture Error", "Error Box", MB_OK);
		return;
	}

	return;
}

void LModel::SetAnimationArraySRV()
{
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	HRESULT hr = LGlobal::g_pDevice->CreateShaderResourceView(pAnimationArrayTex.Get(), &srvDesc, pAnimationArraySRV.GetAddressOf());

	if (FAILED(hr))
	{
		MessageBoxA(NULL, "Create AnimationArraySRV 2DTexture Error", "Error Box", MB_OK);
		return;
	}

	LGlobal::g_pImmediateContext->VSSetShaderResources(2, 1, pAnimationArraySRV.GetAddressOf());

	return;
}

void LModel::SetInstancing(bool IsIntancing, int instancingCount)
{
	auto fbxMeshList = m_pModel->m_DrawList;

	for (int iSub = 0; iSub < fbxMeshList.size(); iSub++)
	{
		fbxMeshList[iSub]->SetInstancing(IsIntancing);
		fbxMeshList[iSub]->CreateInstance(instancingCount);
	}
}

void LModel::SetLFbxObj(LFbxObj* fbxObj)
{
	m_pModel = fbxObj;
}

LFbxObj* LModel::GetLFbxObj()
{
	return m_pModel;
}

bool LModel::Init(int animationIndex)
{
	if (m_pActionModel == nullptr) return false;

	auto fbxMeshList = m_pModel->m_DrawList;

	for (int iSub = 0; iSub < fbxMeshList.size(); iSub++)
	{
		LFbxObj* obj = fbxMeshList[iSub].get();

		for (int frame = 0; frame < m_AnimationFrameList[animationIndex]; frame++)
		{
			for (int iNode = 0; iNode < m_pActionModel->m_AnimationTreeSize; iNode++)
			{
				std::wstring name = m_pModel->m_pFbxNodeNameList[iNode];
				auto model = obj->m_dxMatrixBindPoseMap.find(name);

				if (model == obj->m_dxMatrixBindPoseMap.end())
				{
					continue;
				}

				TMatrix matBindPose = model->second;
				int iIndex = m_pModel->m_pFbxNodeNameMap[name];
				
				m_texBoneArray[m_texAnimationOffset[animationIndex] + frame].matBoneWorld[iIndex] = matBindPose * m_pActionModel->m_NameMatrixMap[frame][name];

				// ��ġ�� �̹� �Ǿ��ִ� 
				/*D3DXMatrixTranspose(&m_texBoneArray[m_texAnimationOffset[m_offsetCount] + frame].matBoneWorld[iIndex],
					&m_texBoneArray[m_texAnimationOffset[m_offsetCount] + frame].matBoneWorld[iIndex]);*/
			}
		}
	}

	return true;
}

bool LModel::ComputeOffset()
{
	int offsetCount = 0;
	int animationIndex = 0;
	m_texAnimationOffset.push_back(offsetCount);

	for (auto iter = LFbxMgr::GetInstance().m_map.begin(); iter != LFbxMgr::GetInstance().m_map.end(); iter++)
	{
		if (iter == LFbxMgr::GetInstance().m_map.begin()) continue;

		iter->second->m_iStartFrame = offsetCount;
		offsetCount += iter->second->m_iEndFrame;
		m_AnimationFrameList.push_back(iter->second->m_iEndFrame);
		iter->second->m_iEndFrame = offsetCount;
		m_texAnimationOffset.push_back(offsetCount);
		m_texBoneArray.resize(offsetCount);
		m_pActionModel = iter->second.get();
		Init(animationIndex);

		animationIndex++;
	}

	m_AllAnimationFrame = offsetCount;

	return true;
}
 
bool LModel::SetTexture()
{
	if (m_pActionModel == nullptr) return false;

	for (int iFrame = 0; iFrame < m_pActionModel->m_iEndFrame; iFrame++)
	{
		for (int iNode = 0; iNode < m_pActionModel->m_AnimationTreeSize; iNode++)
		{
			std::wstring name = m_pModel->m_pFbxNodeNameList[iNode];
			m_texBoneArray[iFrame].matBoneWorld[iNode] = m_pActionModel->m_NameMatrixMap[iFrame][name];
		}
	}

	auto fbxMeshList = m_pModel->m_DrawList;

	for (int iSub = 0; iSub < fbxMeshList.size(); iSub++)
	{
		LFbxObj* obj = fbxMeshList[iSub].get();

		for (int iFrame = 0; iFrame < m_pActionModel->m_iEndFrame; iFrame++)
		{
			for (auto data : m_pModel->m_pFbxNodeNameMap)
			{
				auto model = obj->m_dxMatrixBindPoseMap.find(data.first);

				if (model == obj->m_dxMatrixBindPoseMap.end())
				{
					continue;
				}

				TMatrix matBindPose = model->second;
				int iIndex = data.second;
				m_texBoneArray[iFrame].matBoneWorld[iIndex] = matBindPose * m_texBoneArray[iFrame].matBoneWorld[iIndex];
			}
		}
	}

	return true;
}

bool LModel::CreateBoneBuffer()
{
	return true;
}

bool LModel::Frame()
{
	if (m_pModel == nullptr) return false;

	m_fCurrentAnimTime += m_pModel->m_iFrameSpeed * LGlobal::g_fSPF * 1.0f;

	if (m_fCurrentAnimTime >= m_pModel->m_iEndFrame)
	{
		m_fCurrentAnimTime = m_pModel->m_iStartFrame;
	}

	return true;
}

bool LModel::FrameInstancing()
{
	if (m_pModel == nullptr) return false;

	auto fbxMeshList = m_pModel->m_DrawList;
	for (int iSub = 0; iSub < fbxMeshList.size(); iSub++)
	{
		LFbxObj* obj = fbxMeshList[iSub].get();
		for (int j = 0; j < obj->m_InstanceSize; j++)
		{
			obj->m_matInstanceList.mat[j] = m_matControl;
			D3DXMatrixTranspose(&obj->m_matInstanceList.mat[j], &obj->m_matInstanceList.mat[j]);
		}

		LGlobal::g_pImmediateContext->UpdateSubresource(obj->m_pCBInstance.Get(), 0, NULL, &(obj->m_matInstanceList), 0, 0);
		LGlobal::g_pImmediateContext->VSSetConstantBuffers(3, 1, obj->m_pCBInstance.GetAddressOf());
	}

	return true;
}

bool LModel::Render()
{
	if (m_pModel == nullptr) return false;

	auto fbxMeshList = m_pModel->m_DrawList;

	for (int iSub = 0; iSub < fbxMeshList.size(); iSub++)
	{
		LFbxObj* obj = fbxMeshList[iSub].get();
		//TMatrix matWorld = m_pModel->m_MatrixArray[m_fCurrentAnimTime][obj->m_iBoneIndex] * m_matControl;
		obj->SetMatrix(&m_matControl, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
		obj->PreRender();
		obj->PostRender();
	}

	return true;
}

bool LModel::ShadowRender(TMatrix* matShadowWorld, TMatrix* matShadowView, TMatrix* matShadowProj)
{
	if (m_pModel == nullptr) return false;

	auto fbxMeshList = m_pModel->m_DrawList;

	for (int iSub = 0; iSub < fbxMeshList.size(); iSub++)
	{
		LFbxObj* obj = fbxMeshList[iSub].get();
		//TMatrix matWorld = m_pModel->m_MatrixArray[m_fCurrentAnimTime][obj->m_iBoneIndex] * m_matControl;
		obj->SetMatrix(matShadowWorld, matShadowView, matShadowProj);
		obj->PreRender();
		obj->PostRender();
	}

	return true;
}

bool LModel::Release()
{
	return true;
}

bool LSkinningModel::CreateBoneBuffer()
{
	D3D11_BUFFER_DESC bufferDesc{};
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	bufferDesc.ByteWidth = sizeof(LBoneWorld);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	HRESULT hr = LGlobal::g_pDevice->CreateBuffer(
		&bufferDesc,
		nullptr,
		m_pBoneArrayCB.GetAddressOf());

	if (FAILED(hr))
	{
		MessageBoxA(NULL, "Create ConstantBuffer Error", "Error Box", MB_OK);
		return false;
	}

	return true;
}

bool LSkinningModel::CreateCurrentFrameBuffer()
{
	D3D11_BUFFER_DESC bufferDesc{};
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	bufferDesc.ByteWidth = sizeof(m_CurrentFrame);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	HRESULT hr = LGlobal::g_pDevice->CreateBuffer(
		&bufferDesc,
		nullptr,
		m_pCurrentFrameCB.GetAddressOf());

	if (FAILED(hr))
	{
		MessageBoxA(NULL, "Create ConstantBuffer Error", "Error Box", MB_OK);
		return false;
	}

	return true;
}

bool LSkinningModel::AllNodeRender()
{
	for (int iNode = 0; iNode < m_pModel->m_TreeList.size(); iNode++)
	{
		LFbxObj* pFbxNode = m_pModel->m_TreeList[iNode].get();
		if (pFbxNode->m_Tex == nullptr) continue;

		m_matBoneArray.matBoneWorld[iNode] = m_pModel->m_MatrixArray[m_fCurrentAnimTime][iNode];
		D3DXMatrixTranspose(&m_matBoneArray.matBoneWorld[iNode],
			&m_matBoneArray.matBoneWorld[iNode]);
		pFbxNode->SetMatrix(&m_matBoneArray.matBoneWorld[iNode],
			&LGlobal::g_pMainCamera->m_matView,
			&LGlobal::g_pMainCamera->m_matProj);
		pFbxNode->PreRender();
		pFbxNode->PostRender();
	}
	return true;
}

bool LSkinningModel::Frame()
{
	if (m_pActionModel == nullptr) return false;
	m_TimerEnd = false;

	if (m_TimerStart)
	{
		m_fCurrentAnimTime = 0.0f;
		m_TimerStart = false;
	}

	m_fCurrentAnimTime += m_pModel->m_iFrameSpeed * LGlobal::g_fSPF * 1.0f;

	if (m_fCurrentAnimTime >= m_pActionModel->m_iEndFrame)
	{
		m_fCurrentAnimTime = m_pActionModel->m_iStartFrame;
		m_TimerEnd = true;
	}

	for (int iNode = 0; iNode < m_pActionModel->m_AnimationTreeSize; iNode++)
	{
		std::wstring name = m_pModel->m_pFbxNodeNameList[iNode];
		m_matBoneArray.matBoneWorld[iNode] = m_pActionModel->m_NameMatrixMap[int(m_fCurrentAnimTime)][name];
	}

	return true;
}

bool LSkinningModel::AniFrame()
{
	if (m_pActionModel == nullptr) return false;
	m_TimerEnd = false;

	if (m_TimerStart)
	{
		m_fCurrentAnimTime = m_pActionModel->m_iStartFrame;
		m_TimerStart = false;
	}

	m_fCurrentAnimTime += m_pModel->m_iFrameSpeed * LGlobal::g_fSPF * 1.0f;

	if (m_fCurrentAnimTime >= m_pActionModel->m_iEndFrame)
	{
		m_fCurrentAnimTime = m_pActionModel->m_iStartFrame;
		m_TimerEnd = true;
	}

	m_CurrentFrame[0] = m_fCurrentAnimTime;

	LGlobal::g_pImmediateContext->UpdateSubresource(m_pCurrentFrameCB.Get(), 0, NULL, &m_CurrentFrame, 0, 0);
	LGlobal::g_pImmediateContext->VSSetConstantBuffers(2, 1, m_pCurrentFrameCB.GetAddressOf());
	return true;
}

bool LSkinningModel::Render()
{
	if (m_pModel == nullptr) return false;

	auto fbxMeshList = m_pModel->m_DrawList;

	for (int iSub = 0; iSub < fbxMeshList.size(); iSub++)
	{
		LFbxObj* obj = fbxMeshList[iSub].get();

		for (auto data : m_pModel->m_pFbxNodeNameMap)
		{
			auto model = obj->m_dxMatrixBindPoseMap.find(data.first);

			if (model == obj->m_dxMatrixBindPoseMap.end())
			{
				continue;
			}

			TMatrix matBindPose = model->second;
			int iIndex = data.second;
			m_matMeshBoneArray.matBoneWorld[iIndex] = matBindPose * m_matBoneArray.matBoneWorld[iIndex];

			// hlsl���� dx�������� ������ ��ġ���Ѽ� �Ѱ���� �Ѵ�
			D3DXMatrixTranspose(&m_matMeshBoneArray.matBoneWorld[iIndex],
				&m_matMeshBoneArray.matBoneWorld[iIndex]);
		}

		// 71���߿� �ش��ϴ� ���� ���õ� �ִϸ��̼� ��ĸ� CB�� �Ѱ���
		LGlobal::g_pImmediateContext->UpdateSubresource(m_pBoneArrayCB.Get(), 0, NULL, &m_matMeshBoneArray, 0, 0);
		LGlobal::g_pImmediateContext->VSSetConstantBuffers(1, 1, m_pBoneArrayCB.GetAddressOf());

		obj->SetMatrix(&m_matControl,
			&LGlobal::g_pMainCamera->m_matView,
			&LGlobal::g_pMainCamera->m_matProj);

		// ���� IA�����̶� �ؽ�ó ������ ���⼭ �Ѵ�.
		obj->PreRender();

		// ���ؽ� ���۸� �ΰ� ����Ѵٰ� ������ �� �Ѱ���
		UINT Strides[2] = { sizeof(SimpleVertex), sizeof(LVertexIW) };
		UINT Offsets[2] = { 0, };

		ComPtr<ID3D11Buffer> buffer[2] = { obj->m_pVertexBuffer.Get(), obj->m_pVBWeightList.Get() };
		LGlobal::g_pImmediateContext->IASetVertexBuffers(0, 2, buffer->GetAddressOf(), Strides, Offsets);

		obj->PostRender();
		// ������ �� �� ī�޶� �����̸� ī�޶��� ������� ������
	}

	return true;
}

bool LSkinningModel::AniRender()
{
	if (m_pModel == nullptr) return false;

	auto fbxMeshList = m_pModel->m_DrawList;

	for (int iSub = 0; iSub < fbxMeshList.size(); iSub++)
	{
		LFbxObj* obj = fbxMeshList[iSub].get();

		obj->SetMatrix(&m_matControl,
			&LGlobal::g_pMainCamera->m_matView,
			&LGlobal::g_pMainCamera->m_matProj);

		// ���� IA�����̶� �ؽ�ó ������ ���⼭ �Ѵ�.
		obj->PreRender();

		// ���ؽ� ���۸� �ΰ� ����Ѵٰ� ������ �� �Ѱ���
		UINT Strides[2] = { sizeof(SimpleVertex), sizeof(LVertexIW) };
		UINT Offsets[2] = { 0, };

		ComPtr<ID3D11Buffer> buffer[2] = { obj->m_pVertexBuffer.Get(), obj->m_pVBWeightList.Get() };
		LGlobal::g_pImmediateContext->IASetVertexBuffers(0, 2, buffer->GetAddressOf(), Strides, Offsets);

		obj->PostRender();
		// ������ �� �� ī�޶� �����̸� ī�޶��� ������� ������
	}

	return true;
}

bool LSkinningModel::Release()
{
	return true;
}