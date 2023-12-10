#include "LSelect.h"
#include "LGlobal.h"
#include "LInput.h"
#include "LWrite.h"

void LSelect::SetMatrix(TMatrix* pWorld, TMatrix* pView, TMatrix* pProj)
{
	if (pWorld != nullptr)
	{
		m_matWorld = *pWorld;
	}

	if (pView != nullptr)
	{
		m_matView = *pView;
	}

	if (pProj != nullptr)
	{
		m_matProj = *pProj;
	}

	Update();
}

void LSelect::Update()
{
	POINT ptCursor;
	GetCursorPos(&ptCursor);
	ScreenToClient(LGlobal::g_hWnd, &ptCursor);

	TVector3 v;
	v.x = (((2.0f * ptCursor.x) / LGlobal::g_WindowWidth) - 1) / m_matProj._11;
	v.y = -(((2.0f * ptCursor.y) / LGlobal::g_WindowHeight) - 1) / m_matProj._22;
	v.z = 1.0f;

	TMatrix mWorldView = m_matWorld * m_matView;
	TMatrix m;
	D3DXMatrixInverse(&m, NULL, &mWorldView);

	m_Ray.vOrigin = TVector3(0.0f, 0.0f, 0.0f);
	m_Ray.vDirection = TVector3(v.x, v.y, v.z);
	D3DXVec3TransformCoord(&m_Ray.vOrigin, &m_Ray.vOrigin, &m);
	D3DXVec3TransformNormal(&m_Ray.vDirection, &m_Ray.vDirection, &m);
	D3DXVec3Normalize(&m_Ray.vDirection, &m_Ray.vDirection);
}

TVector3 LSelect::GetIntersection()
{
	return m_vIntersection;
}

bool LSelect::GetIntersection(TVector3 vStart, TVector3 vEnd, TVector3 vNormal, TVector3 v0, TVector3 v1, TVector3 v2, float* fPercentage)
{
	TVector3 vDirection = vEnd - vStart;
	// 법선벡터와 반직선 내적
	// 내적하면 삼각형의 법선벡터와 얼마나 비슷한지 성분이 나온다.
	// 그림자는 법선벡터와 비슷하게 삼각형의 위를 향해있다.
	float D = D3DXVec3Dot(&vNormal, &vDirection);
	// 레이의 시작 지점과 삼각형의 한점을 내적하면
	// 법선벡터 보다 작지만 삼각형을 덮고 살짝 올라가는 그림자가진다.
	TVector3 alen = v0 - vStart;
	float a0 = D3DXVec3Dot(&vNormal, &alen);
	float fT = a0 / D;

	if (fPercentage != nullptr)
	{
		*fPercentage = fT;
	}

	if (fT < 0.0f || fT > 1.0f)
	{
		return false;
	}

	m_vIntersection = vStart + vDirection * fT;

	return true;
}

bool LSelect::GetIntersection(float fRayRange, TVector3 vNormal, TVector3 v0, TVector3 v1, TVector3 v2, float* fPercentage)
{
	TVector3 vDirection = (m_Ray.vDirection * fRayRange) - m_Ray.vOrigin;
	// 법선벡터와 반직선 내적
	// 내적하면 삼각형의 법선벡터와 얼마나 비슷한지 성분이 나온다.
	// 그림자는 법선벡터와 비슷하게 삼각형의 위를 향해있다.
	float D = D3DXVec3Dot(&vNormal, &vDirection);
	// 레이의 시작 지점과 삼각형의 한점을 내적하면
	// 법선벡터 보다 작지만 삼각형을 덮고 살짝 올라가는 그림자가진다.
	TVector3 alen = v0 - m_Ray.vOrigin;
	float a0 = D3DXVec3Dot(&vNormal, &alen);
	float fT = a0 / D;

	if (fPercentage != nullptr)
	{
		*fPercentage = fT;
	}

	if (fT < 0.0f || fT > 1.0f)
	{
		return false;
	}

	m_vIntersection = m_Ray.vOrigin + vDirection * fT;

	return true;
}

bool LSelect::PointInPolygon(TVector3 iVertex, TVector3 faceNormal, TVector3 v0, TVector3 v1, TVector3 v2)
{
	TVector3 e0, e1, iInter, vNormal;

	e0 = v2 - v1;
	e1 = v0 - v1;
	iInter = iVertex - v1;

	D3DXVec3Cross(&vNormal, &e0, &iInter);
	D3DXVec3Normalize(&vNormal, &vNormal);
	float fDot = D3DXVec3Dot(&faceNormal, &vNormal);
	if (fDot < 0.0f) return false;

	D3DXVec3Cross(&vNormal, &iInter, &e1);
	D3DXVec3Normalize(&vNormal, &vNormal);
	fDot = D3DXVec3Dot(&faceNormal, &vNormal);
	if (fDot < 0.0f) return false;

	e0 = v0 - v2;
	e1 = v1 - v2;
	iInter = iVertex - v2;

	D3DXVec3Cross(&vNormal, &e0, &iInter);
	D3DXVec3Normalize(&vNormal, &vNormal);
	fDot = D3DXVec3Dot(&faceNormal, &vNormal);
	if (fDot < 0.0f) return false;

	D3DXVec3Cross(&vNormal, &iInter, &e1);
	D3DXVec3Normalize(&vNormal, &vNormal);
	fDot = D3DXVec3Dot(&faceNormal, &vNormal);
	if (fDot < 0.0f) return false;

	return true;
}

bool LSelect::ObjectIsIntersection(LObject* obj)
{
	TVector3 v0, v1, v2;

	for (int iFace = 0; iFace < 1; iFace++)
	{
		v0 = obj->m_VertexList[iFace * 3 + 0].p;
		v1 = obj->m_VertexList[iFace * 3 + 1].p;
		v2 = obj->m_VertexList[iFace * 3 + 2].p;

		TVector3 vNormal, vCross;
		TVector3 e0 = v1 - v0;
		TVector3 e1 = v2 - v0;
		D3DXVec3Cross(&vCross, &e0, &e1);
		D3DXVec3Normalize(&vNormal, &vCross);

		if (GetIntersection(
			100.0f,
			vNormal,
			v0, v1, v2) && LInput::GetInstance().m_dwKeyState[VK_LBUTTON])
		{
			if (PointInPolygon(m_vIntersection, vNormal, v0, v1, v2))
			{
				std::wstring text = L"Click to Face";
				LWrite::GetInstance().AddText(text, 0.0f, 100.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
				return true;
			}
		}
	}
}

LSelect::LSelect()
{
	m_vIntersection = TVector3(0, 0, 0);

	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
}

LSelect::~LSelect()
{

}