#include "LModel.h"

void LModel::SetLFbxObj(LFbxObj* fbxObj)
{
	m_LFbxObj = fbxObj;
}

void LModel::SetChild()
{
	// 메쉬 갯수만큼 순환
	for (int iMesh = 0; iMesh < m_LFbxObj->m_MeshList.size(); iMesh++)
	{
		// Render를 위한 세팅
		std::shared_ptr<LModel> model = std::make_shared<LModel>();
		model->Set();
		// SubModel마다 Mesh세팅
		model->m_Mesh = m_LFbxObj->m_MeshList[iMesh];

		// 기본 행렬 프레임 0의 행렬 세팅
		memcpy(&model->m_matWorld, &model->m_Mesh->m_MatWorld, sizeof(TMatrix));

		// model의 VertexList 미리 공간 확보
		model->m_VertexList.resize(m_LFbxObj->m_MeshList[iMesh]->m_NumPoly * 3);

		UINT iOffset = 0;
		// 트라이앵글리스트의 수만큼 순환
		for (int iSubMtrl = 0; iSubMtrl < m_LFbxObj->m_MeshList[iMesh]->m_TriangleList.size(); iSubMtrl++)
		{
			m_LFbxObj->m_MeshList[iMesh]->m_TriangleOffsetList.push_back(iOffset);

			// 해당하는 자식의 VertexList에 mesh의 정점을 전달해줌
			for (int iVertex = 0; iVertex < m_LFbxObj->m_MeshList[iMesh]->m_TriangleList[iSubMtrl].size(); iVertex++)
			{
				model->m_VertexList[iVertex + iOffset] = m_LFbxObj->m_MeshList[iMesh]->m_TriangleList[iSubMtrl][iVertex];
			}

			iOffset += m_LFbxObj->m_MeshList[iMesh]->m_TriangleList[iSubMtrl].size();
		}

		model->Create(L"../../res/hlsl/CustomizeMap.hlsl", L"../../res/map/topdownmap.jpg");
		m_SubModel.push_back(model);
	}
}

void LModel::SetAnimationSpeed(float speed)
{
	m_AnimationSpeed = speed;
}

float LModel::GetAnimationSpeed()
{
	return m_AnimationSpeed;
}

LFbxObj* LModel::GetLFbxObj()
{
	return m_LFbxObj;
}

LMesh* LModel::GetMesh()
{
	return m_Mesh.get();
}

bool LModel::PostRender() 
{
	for (int isubMtrl = 0; isubMtrl < m_Mesh->m_TriangleList.size(); isubMtrl++)
	{
		if (m_Mesh->m_TexArray.size() && m_Mesh->m_TexArray[isubMtrl]->m_pTexSRV)
		{
			m_Mesh->m_TexArray[isubMtrl]->Apply();
		}

		if (m_pIndexBuffer == nullptr)
		{
			m_pImmediateContext->Draw(m_Mesh->m_TriangleList[isubMtrl].size(),
				m_Mesh->m_TriangleOffsetList[isubMtrl]);
		}
	}

	return true;
}