#include "LBackView.h"
#include "LInput.h"
#include "LGlobal.h"

void LBackView::SetTarget(LModel* model)
{
	m_TargetModel = model;
	m_ModelMatrix = model->m_matControl;
}

bool LBackView::Init()
{
	LCamera::Init();
	return true;
}

bool LBackView::Frame()
{
	float x = LInput::GetInstance().m_vOffset.x;
	float y = LInput::GetInstance().m_vOffset.y;
	m_fCameraYaw += DirectX::XMConvertToRadians(x * 0.1f);
	m_fCameraPitch += DirectX::XMConvertToRadians(y * 0.1f);

	DirectX::XMVECTOR gRotation;
	DirectX::XMMATRIX matRotation;
	DirectX::XMMATRIX xmMatView;
	DirectX::XMMATRIX xmInverse;
	TMatrix matTemp;

	//gRotation = DirectX::XMQuaternionRotationRollPitchYaw(m_fCameraPitch, m_fCameraYaw, 0);

	//DirectX::XMVECTOR xmCameraPos = DirectX::XMVectorSet(m_vCameraPos.x, m_vCameraPos.y, m_vCameraPos.z, 1.0f);
	//matRotation = DirectX::XMMatrixAffineTransformation(DirectX::g_XMOne, DirectX::g_XMZero, gRotation, xmCameraPos);
	//xmMatView = DirectX::XMMatrixInverse(NULL, matRotation);
	//std::memcpy(&m_matView, &xmMatView, sizeof(DirectX::XMMATRIX));

	gRotation = DirectX::XMQuaternionRotationRollPitchYaw(m_fCameraPitch, m_fCameraYaw, 0);
	DirectX::XMVECTOR xmModelPos = DirectX::XMVectorSet(m_TargetModel->m_matControl._41, m_TargetModel->m_matControl._42, m_TargetModel->m_matControl._43, 1.0f);
	matRotation = DirectX::XMMatrixAffineTransformation(DirectX::g_XMOne, DirectX::g_XMZero, gRotation, xmModelPos);
	
	matTemp = m_ModelMatrix * matRotation;
	//std::memcpy(&matTemp, &xmInverse, sizeof(DirectX::XMMATRIX));
	m_TargetModel->m_matControl = matTemp;

	m_fCameraPitch = max(-m_MaxPitch, min(m_MaxPitch, m_fCameraPitch));
	m_vCameraPos.x = m_TargetModel->m_matControl._41;
	if (m_vCameraPos.x < -970.0f)
	{
		m_vCameraPos.x = -970.0f;
	}
	if (m_vCameraPos.x > 970.0f)
	{
		m_vCameraPos.x = 970.0f;
	}
	m_vCameraPos.y = m_TargetModel->m_matControl._42;
	m_vCameraPos.z = m_TargetModel->m_matControl._43;
	if (m_vCameraPos.z < -970.0f)
	{
		m_vCameraPos.z = -970.0f;
	}
	if (m_vCameraPos.z > 970.0f)
	{
		m_vCameraPos.z = 970.0f;
	}
	TVector3 backView = m_TargetModel->m_matControl.Forward() * 100.f;
	m_vCameraPos -= backView;
	m_vCameraPos.y += 35.f;
	CreateLookAt(m_vCameraPos, m_vCameraPos + backView);

	UpdateVector();

	return true;
}

bool LBackView::Render()
{
	LCamera::Render();
	return true;
}

bool LBackView::Release()
{
	LCamera::Release();
	return true;
}