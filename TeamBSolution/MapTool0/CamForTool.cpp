#include "CamForTool.h"

bool CamForTool::Frame()
{
	DirectX::XMVECTOR gRotation;
	DirectX::XMMATRIX matRotation;
	DirectX::XMMATRIX xmMatView;

	gRotation = DirectX::XMQuaternionRotationRollPitchYaw(m_fCameraPitch, m_fCameraYaw, 0);

	DirectX::XMVECTOR xmCameraPos = DirectX::XMVectorSet(m_vCameraPos.x, m_vCameraPos.y, m_vCameraPos.z, 1.0f);
	matRotation = DirectX::XMMatrixAffineTransformation(DirectX::g_XMOne, DirectX::g_XMZero, gRotation, xmCameraPos);
	xmMatView = DirectX::XMMatrixInverse(NULL, matRotation);
	std::memcpy(&m_matView, &xmMatView, sizeof(DirectX::XMMATRIX));

	UpdateVector();
	return true;
}
