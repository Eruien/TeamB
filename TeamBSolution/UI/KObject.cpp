#include "KObject.h"
#include "Animator.h"
bool KObject::Init()
{
	for (shared_ptr<MonoBehaviour>& script : _scripts)
	{
		script->Init();
	}
		Set();
		LObject::Init();
		TMatrix matScale, matRotation, matTranslation;
		matScale.CreateScale(m_vScale);
		matRotation.CreateRotationZ(m_vRotation.z);
		matTranslation.Translation(m_vPosition);
		m_matWorld = matScale * matRotation * matTranslation;
		return true;
}

bool KObject::Frame()
{
	SetRect(m_vPosition, m_vScale);

	for (shared_ptr<MonoBehaviour>& script : _scripts)
	{
		script->Frame();
	}



	TMatrix matScale, matRotation, matTranslation;
	matScale = matScale.CreateScale(m_vScale);
	matRotation = matRotation.CreateRotationX(m_vRotation.x);
	matRotation = matRotation.CreateRotationY(m_vRotation.y);
	matRotation = matRotation.CreateRotationZ(m_vRotation.z);
	matTranslation.Translation(m_vPosition);
	m_matWorld = matScale * matRotation * matTranslation;
	
	
	return true;
}

bool KObject::Render()
{
	for (shared_ptr<MonoBehaviour>& script : _scripts)
	{
		script->Render();
	}
	PreRender();
	
	PostRender();
	return false;
}

bool KObject::Release()
{
	return false;
}


void KObject::SetRect(TVector3 pos, TVector3 scale)
{
	_rect = TRectangle(pos.x, pos.y, scale.x, scale.y);


}

bool KObject::CreateVertexBuffer()
{
	m_VertexList.resize(6);

	m_VertexList[0].p.x = -0.5f; m_VertexList[0].p.y = 0.5f;  m_VertexList[0].p.z = 0.0f;
	m_VertexList[1].p.x = 0.5f;  m_VertexList[1].p.y = 0.5f;  m_VertexList[1].p.z = 0.0f;
	m_VertexList[2].p.x = -0.5f; m_VertexList[2].p.y = -0.5f; m_VertexList[2].p.z = 0.0f;

	m_VertexList[3].p.x = -0.5f; m_VertexList[3].p.y = -0.5f; m_VertexList[3].p.z = 0.0f;
	m_VertexList[4].p.x = 0.5f;  m_VertexList[4].p.y = 0.5f;  m_VertexList[4].p.z = 0.0f;
	m_VertexList[5].p.x = 0.5f;  m_VertexList[5].p.y = -0.5f; m_VertexList[5].p.z = 0.0f;

	m_VertexList[0].t.x = 0.0f;   m_VertexList[0].t.y = 0.0f;
	m_VertexList[1].t.x = 1.0f;  m_VertexList[1].t.y = 0.0f;
	m_VertexList[2].t.x = 0.0f;   m_VertexList[2].t.y = 1.0f;

	m_VertexList[3].t.x = 0.0f;   m_VertexList[3].t.y = 1.0f;
	m_VertexList[4].t.x = 1.0f;  m_VertexList[4].t.y = 0.0f;
	m_VertexList[5].t.x = 1.0f;  m_VertexList[5].t.y = 1.0f;

	// 버퍼에 대한 설명서
	D3D11_BUFFER_DESC bufferDesc = { 0, };
	bufferDesc.ByteWidth = sizeof(SimpleVertex) * m_VertexList.size();
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA initialData = { 0, };
	initialData.pSysMem = &m_VertexList.at(0);

	HRESULT hr = m_pDevice->CreateBuffer(
		&bufferDesc,
		&initialData,
		&m_pVertexBuffer);

	if (FAILED(hr))
	{
		MessageBoxA(NULL, "Create Buffer Error", "Error Box", MB_OK);
		return false;
	}

	return true;
}

void KObject::SetAnimation()
{
}

