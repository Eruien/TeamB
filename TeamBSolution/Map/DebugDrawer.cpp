#include "DebugDrawer.h"


DebugDrawer::DebugDrawer()
{
	shader = std::make_shared<Shader>(L"MapToolShader/DebugShader.fx");
}

DebugDrawer::~DebugDrawer()
{
}

void DebugDrawer::DrawLine(TVector3 start, TVector3 end, Color color)
{
	std::vector<SimpleVertex> vertices;
	vertices.resize(2);

	vertices[0].p = start;
	vertices[0].c = color;
	vertices[1].p = end;
	vertices[1].c = color;

	std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>();
	vertexBuffer->CreateVertexBuffer(vertices);
	vertexBufferList.push_back(vertexBuffer);

	std::vector<UINT> indices;
	indices.resize(2);
	indices[0] = 0;
	indices[1] = 1;
	std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>();
	indexBuffer->CreateIndexBuffer(indices);
	indexBufferList.push_back(indexBuffer);
}

void DebugDrawer::DrawBox(Cube box, Color color)
{
	std::vector<SimpleVertex> vtx(24);



	float w2 = 0.5f;
	float h2 = 0.5f;
	float d2 = 0.5f;

	//  0   1   4   5
	//
	//  2   3   6   7
	TVector3 c0, c1, c2, c3, c4, c5, c6, c7;
	TVector3 size = box.size;

	c0 = box.min; c0.y += size.y;
	c1 = box.min; c1.x += size.x; c1.y += size.y;
	c2 = box.min;
	c3 = box.min; c3.x += size.x;
	c4 = box.min; c4.z += size.z; c4.y += size.y;
	c5 = box.max;
	c6 = box.min; c6.z += size.z;
	c7 = box.max; c7.y -= size.y;

	// �ո�
	vtx[0] = SimpleVertex(c0, TVector2(0.0f, 0.0f), color, TVector3(0.0f, 0.0f, 0.0f));
	vtx[1] = SimpleVertex(c1, TVector2(0.0f, 0.0f), color, TVector3(0.0f, 0.0f, 0.0f));
	vtx[2] = SimpleVertex(c3, TVector2(0.0f, 0.0f), color, TVector3(0.0f, 0.0f, 0.0f));
	vtx[3] = SimpleVertex(c2, TVector2(0.0f, 0.0f), color, TVector3(0.0f, 0.0f, 0.0f));
	// �޸�
	vtx[4] = SimpleVertex(c4, TVector2(0.0f, 0.0f), color, TVector3(0.0f, 0.0f, 0.0f));
	vtx[5] = SimpleVertex(c5, TVector2(0.0f, 0.0f), color, TVector3(0.0f, 0.0f, 0.0f));
	vtx[6] = SimpleVertex(c7, TVector2(0.0f, 0.0f), color, TVector3(0.0f, 0.0f, 0.0f));
	vtx[7] = SimpleVertex(c6, TVector2(0.0f, 0.0f), color, TVector3(0.0f, 0.0f, 0.0f));
	// ����
	vtx[8] = SimpleVertex(c4, TVector2(0.0f, 0.0f), color, TVector3(0.0f, 0.0f, 0.0f));
	vtx[9] = SimpleVertex(c5, TVector2(0.0f, 0.0f), color, TVector3(0.0f, 0.0f, 0.0f));
	vtx[10] = SimpleVertex(c1, TVector2(0.0f, 0.0f), color, TVector3(0.0f, 0.0f, 0.0f));
	vtx[11] = SimpleVertex(c0, TVector2(0.0f, 0.0f), color, TVector3(0.0f, 0.0f, 0.0f));
	// �Ʒ���
	vtx[12] = SimpleVertex(c2, TVector2(0.0f, 0.0f), color, TVector3(0.0f, 0.0f, 0.0f));
	vtx[13] = SimpleVertex(c3, TVector2(0.0f, 0.0f), color, TVector3(0.0f, 0.0f, 0.0f));
	vtx[14] = SimpleVertex(c7, TVector2(0.0f, 0.0f), color, TVector3(0.0f, 0.0f, 0.0f));
	vtx[15] = SimpleVertex(c6, TVector2(0.0f, 0.0f), color, TVector3(0.0f, 0.0f, 0.0f));
	// ���ʸ�
	vtx[16] = SimpleVertex(c4, TVector2(0.0f, 0.0f), color, TVector3(0.0f, 0.0f, 0.0f));
	vtx[17] = SimpleVertex(c0, TVector2(0.0f, 0.0f), color, TVector3(0.0f, 0.0f, 0.0f));
	vtx[18] = SimpleVertex(c2, TVector2(0.0f, 0.0f), color, TVector3(0.0f, 0.0f, 0.0f));
	vtx[19] = SimpleVertex(c6, TVector2(0.0f, 0.0f), color, TVector3(0.0f, 0.0f, 0.0f));
	// �����ʸ�
	vtx[20] = SimpleVertex(c1, TVector2(0.0f, 0.0f), color, TVector3(0.0f, 0.0f, 0.0f));
	vtx[21] = SimpleVertex(c5, TVector2(0.0f, 0.0f), color, TVector3(0.0f, 0.0f, 0.0f));
	vtx[22] = SimpleVertex(c7, TVector2(0.0f, 0.0f), color, TVector3(0.0f, 0.0f, 0.0f));
	vtx[23] = SimpleVertex(c3, TVector2(0.0f, 0.0f), color, TVector3(0.0f, 0.0f, 0.0f));

	std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>();
	vertexBuffer->CreateVertexBuffer(vtx);
	vertexBufferList.push_back(vertexBuffer);



	std::vector<UINT> idx(36);
	// �ո�
	idx[0] = 0; idx[1] = 1; idx[2] = 2;
	idx[3] = 0; idx[4] = 2; idx[5] = 3;
	// �޸�
	idx[6] = 4; idx[7] = 5; idx[8] = 6;
	idx[9] = 4; idx[10] = 6; idx[11] = 7;
	// ����
	idx[12] = 8; idx[13] = 9; idx[14] = 10;
	idx[15] = 8; idx[16] = 10; idx[17] = 11;
	// �Ʒ���
	idx[18] = 12; idx[19] = 13; idx[20] = 14;
	idx[21] = 12; idx[22] = 14; idx[23] = 15;
	// ���ʸ�
	idx[24] = 16; idx[25] = 17; idx[26] = 18;
	idx[27] = 16; idx[28] = 18; idx[29] = 19;
	// ������
	idx[30] = 20; idx[31] = 21; idx[32] = 22;
	idx[33] = 20; idx[34] = 22; idx[35] = 23;

	std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>();
	indexBuffer->CreateIndexBuffer(idx);
	indexBufferList.push_back(indexBuffer);
}

void DebugDrawer::DrawRect(std::vector<TVector3>& points, Color color)
{
	std::vector<SimpleVertex> vtx(4);

	vtx[0] = SimpleVertex(points[0], TVector2(0.0f, 0.0f), color, TVector3(0.0f, 0.0f, 0.0f));
	vtx[1] = SimpleVertex(points[1], TVector2(0.0f, 0.0f), color, TVector3(0.0f, 0.0f, 0.0f));
	vtx[2] = SimpleVertex(points[2], TVector2(0.0f, 0.0f), color, TVector3(0.0f, 0.0f, 0.0f));
	vtx[3] = SimpleVertex(points[3], TVector2(0.0f, 0.0f), color, TVector3(0.0f, 0.0f, 0.0f));

	std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>();
	vertexBuffer->CreateVertexBuffer(vtx);
	vertexBufferList.push_back(vertexBuffer);

	std::vector<UINT> idx(6);
	idx[0] = 0; idx[1] = 1; idx[2] = 2;
	idx[3] = 2; idx[4] = 1; idx[5] = 3;

	std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>();
	indexBuffer->CreateIndexBuffer(idx);
	indexBufferList.push_back(indexBuffer);
}

void DebugDrawer::Update()
{
	//ImGui::InputInt("Pass", &pass);

	if (pass < 0)
		pass = 0;

	if (pass > 1)
		pass = 1;

	if (LInput::GetInstance().GetKey(DIK_I) == KEY_PUSH)
	{
		vertexBufferList.clear();
		indexBufferList.clear();
	}
}

void DebugDrawer::Render()
{
	TMatrix world = TMatrix::Identity;
	shader->GetMatrix("World")->SetMatrix((float*)&world);
	/*shader->GetMatrix("View")->SetMatrix((float*)&CameraManager::GetInstance().GetMainCamera()->viewMatrix);
	shader->GetMatrix("Projection")->SetMatrix((float*)&CameraManager::GetInstance().GetMainCamera()->projectionMatrix);*/
	;
	

	shader->GetMatrix("View")->SetMatrix((float*)&LGlobal::g_pMainCamera->m_matView);
	shader->GetMatrix("Projection")->SetMatrix((float*)&LGlobal::g_pMainCamera->m_matProj);

	for (int i = 0; i < vertexBufferList.size(); i++)
	{
		UINT stride = vertexBufferList[i]->GetStride();
		UINT offset = vertexBufferList[i]->GetOffset();

		LGlobal::g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		LGlobal::g_pImmediateContext->IASetVertexBuffers(0, 1, vertexBufferList[i]->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		LGlobal::g_pImmediateContext->IASetIndexBuffer(indexBufferList[i]->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

		shader->DrawIndexed(0, pass, indexBufferList[i]->GetIndexCount());
	}
}
