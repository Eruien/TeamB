#include "LObject.h"
//월드좌표 오브젝트를 월드->뷰->투영해서 핸들에 전달
void  LObject::SetMatrix(TMatrix* matWorld, TMatrix* matView, TMatrix* matProj)
{
	if (matWorld != nullptr)
	{
		m_matWorld = *matWorld;
	}

	if (matView != nullptr)
	{
		m_matView = *matView;
	}

	if (matProj != nullptr)
	{
		m_matProj = *matProj;
	}

	m_cbData.matWorld = m_matWorld.Transpose();
	m_cbData.matView = m_matView.Transpose();
	m_cbData.matProj = m_matProj.Transpose();
	m_pImmediateContext->UpdateSubresource(m_pConstantBuffer.Get(), 0, nullptr, &m_cbData, 0, 0);
}
void LObject::UpdateMatrix()
{	//스케일이 안되서 임시 수정
	//TMatrix matScale, matRotation, matTranslation;
	//matScale.CreateScale(m_vScale);
	//matRotation.CreateRotationZ(m_vRotation.z);
	//matTranslation.Translation(m_vPosition);
	//m_matWorld = matScale * matRotation * matTranslation;
	TBASIS_EX::TMatrix matScale, matRotation, matTranslate;
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixRotationZ(&matRotation, m_vRotation.z);
	D3DXMatrixTranslation(&matTranslate, m_vPosition.x, m_vPosition.y, m_vPosition.z);

	m_matWorld = matScale * matRotation * matTranslate;//로컬1크기를 스케일->회전->이동 = 월드


}

void LObject::SetScale(TVector3 s)
{
	m_vScale = s;
}

void LObject::SetPos(TVector3 p)
{
	m_vPosition = p;
}

bool LObject::Init()
{	
	LDXObject::Init();
	return true;
}

bool LObject::Frame()
{
	LDXObject::Frame();
	return true;
}

void LObject::UpdateMatrix()//이펙트 머지
{	//스케일이 안되서 임시 수정
	TMatrix matScale, matRotation, matTranslation;
	matScale.CreateScale(m_vScale);
	matRotation.CreateRotationZ(m_vRotation.z);
	matTranslation.Translation(m_vPosition);
	m_matWorld = matScale * matRotation * matTranslation;
	//TBASIS_EX::TMatrix matScale, matRotation, matTranslate;
	//D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	//D3DXMatrixRotationZ(&matRotation, m_vRotation.z);
	//D3DXMatrixTranslation(&matTranslate, m_vPosition.x, m_vPosition.y, m_vPosition.z);

	//m_matWorld = matScale * matRotation * matTranslate;//로컬1크기를 스케일->회전->이동 = 월드


}


bool LObject::Render()
{
	LDXObject::Render();
	return true;
}

bool LObject::Release()
{
	LDXObject::Release();
	return true;
}
bool LObject::Create(std::wstring shaderFileName, std::wstring texFileName)
{
	CreateVertexData();//비어있음
	CreateIndexData();//비어있음
	CreateVertexBuffer();
	CreateIndexBuffer();
	CreateConstantBuffer();
	// obj공용 // m_pVS, m_pPS 쉐이더 생성
	m_Shader = LManager<LShader>::GetInstance().Load(shaderFileName);
	CreateLayout();
	// obj공용 // m_pTexSRV 생성
	m_Tex = LManager<LTexture>::GetInstance().Load(texFileName);
	UpdateMatrix();//로컬에서 월드로 변환
	return true;
}
LObject::LObject()
{
	m_vPosition = TVector3(0, 0, 0);
	m_vRotation = TVector3(0, 0, 0);
	m_vScale = TVector3(1, 1, 1);
}

LObject::~LObject() {}
