#include "LModel.h"
#include "LGlobal.h"

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

	m_fCurrentAnimTime += m_pModel->m_iFrameSpeed * LGlobal::g_fSPF * 1.0f;

	if (m_fCurrentAnimTime >= m_pActionModel->m_iEndFrame)
	{
		m_fCurrentAnimTime = m_pActionModel->m_iStartFrame;
	}

	for (int iNode = 0; iNode < m_pActionModel->m_AnimationTreeSize; iNode++)
	{
		std::wstring name = m_pModel->m_pFbxNodeNameList[iNode];
		m_matBoneArray.matBoneWorld[iNode] = m_pActionModel->m_NameMatrixMap[int(m_fCurrentAnimTime)][name];
	}

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

			// hlsl에서 dx기준으로 쓸려면 전치시켜서 넘겨줘야 한다
			D3DXMatrixTranspose(&m_matMeshBoneArray.matBoneWorld[iIndex],
				&m_matMeshBoneArray.matBoneWorld[iIndex]);
		}

		// 71개중에 해당하는 본에 관련된 애니메이션 행렬만 CB에 넘겨줌
		LGlobal::g_pImmediateContext->UpdateSubresource(m_pBoneArrayCB.Get(), 0, NULL, &m_matMeshBoneArray, 0, 0);
		LGlobal::g_pImmediateContext->VSSetConstantBuffers(1, 1, m_pBoneArrayCB.GetAddressOf());

		obj->SetMatrix(&m_matControl,
			&LGlobal::g_pMainCamera->m_matView,
			&LGlobal::g_pMainCamera->m_matProj);

		// 각종 IA세팅이랑 텍스처 적용을 여기서 한다.
		obj->PreRender();

		// 버텍스 버퍼를 두개 사용한다고 세팅할 때 넘겨줌
		UINT Strides[2] = { sizeof(SimpleVertex), sizeof(LVertexIW) };
		UINT Offsets[2] = { 0, };

		ComPtr<ID3D11Buffer> buffer[2] = { obj->m_pVertexBuffer.Get(), obj->m_pVBWeightList.Get() };
		LGlobal::g_pImmediateContext->IASetVertexBuffers(0, 2, buffer->GetAddressOf(), Strides, Offsets);

		obj->PostRender();
		// 렌더링 할 때 카메라가 움직이면 카메라의 역행렬을 곱해줌
	}

	return true;
}

bool LSkinningModel::Release()
{
	return true;
}