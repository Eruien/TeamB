#include "LPick.h"



void LPick::CalcRay()
{
	LCamera* cam = LGlobal::g_pMainCamera;

	POINT ptCursor;
	GetCursorPos(&ptCursor);
	ScreenToClient(LGlobal::g_hWnd, &ptCursor);

	// 
	_ray.origin = TVector3(0.0f, 0.f, 0.f);
	_ray.direction.x = (((2.0f * ptCursor.x) / LGlobal::g_WindowWidth) - 1) / cam->m_matProj._11;
	_ray.direction.y = -(((2.0f * ptCursor.y) / LGlobal::g_WindowHeight) - 1) / cam->m_matProj._22;
	_ray.direction.z = 1.0f;

	TMatrix WorldView = cam->m_matView;
	TMatrix WorldViewInv;
	D3DXMatrixInverse(&WorldViewInv, NULL, &WorldView);

	D3DXVec3TransformCoord(&_ray.origin, &_ray.origin, &WorldViewInv);  // inverse와 행렬곱
	D3DXVec3TransformNormal(&_ray.direction, &_ray.direction, &WorldViewInv);
	D3DXVec3Normalize(&_ray.direction, &_ray.direction);
}

bool LPick::GetIntersection(float fRange, TVector3 vNormal, TVector3 v0, TVector3 v1, TVector3 v2)
{
	TVector3 dir = (_ray.direction * fRange) - _ray.origin;
	float D = D3DXVec3Dot(&vNormal, &dir);

	TVector3 alen = v0 - _ray.origin;
	float a0 = D3DXVec3Dot(&vNormal, &alen);
	float fT = a0 / D;

	// if(fPercentage != nullptr) *fPercentage = fT;

	if (fT < 0.0f || fT > 1.0f)
		return false;

	_vIntersection = _ray.origin + dir * fT;

	return true;
}

bool LPick::MapIsIntersection(LHeightMap* map)
{
	TVector3 v0, v1, v2;
	std::vector<SimpleVertex> list = map->m_VertexList;


	for (int iFace = 0; iFace < map->m_IndexList.size() / 3; ++iFace)
	{
		v0 = list[map->m_IndexList[iFace * 3 + 0]].p;
		v1 = list[map->m_IndexList[iFace * 3 + 1]].p;
		v2 = list[map->m_IndexList[iFace * 3 + 2]].p;

		TVector3 vNormal, vCross;
		TVector3 e0 = v1 - v0;
		TVector3 e1 = v2 - v0;
		D3DXVec3Cross(&vCross, &e0, &e1);
		D3DXVec3Normalize(&vNormal, &vCross);

		if (GetIntersection(100.f, vNormal, v0, v1, v2))
		{
			if (PointInPolygon(_vIntersection, vNormal, v0, v1, v2))
			{
				std::wstring text = L"Click to Face";

				LWrite::GetInstance().AddText(text, 0.0f, 100.0f, { 1.f, 1.f, 1.f, 1.f, });
				return true;
			}
		}
	}
	return false;
}

bool LPick::PointInPolygon(TVector3 vertex, TVector3 faceNormal, TVector3 v0, TVector3 v1, TVector3 v2)
{
	TVector3 e0, e1, inter, normal;

	e0 = v2 - v1;
	e1 = v0 - v1;
	inter = vertex - v1;

	D3DXVec3Cross(&normal, &e0, &inter);
	D3DXVec3Normalize(&normal, &normal);
	float dot = D3DXVec3Dot(&faceNormal, &normal);
	if (dot < 0.0f) return false;

	D3DXVec3Cross(&normal, &inter, &e1);
	D3DXVec3Normalize(&normal, &normal);
	dot = D3DXVec3Dot(&faceNormal, &normal);
	if (dot < 0.0f) return false;


	e0 = v0 - v2;
	v1 = v1 - v2;
	inter = vertex - v2;

	// 판정하는 코드는 윗 부분과 완전히 동일
	D3DXVec3Cross(&normal, &e0, &inter);
	D3DXVec3Normalize(&normal, &normal);
	dot = D3DXVec3Dot(&faceNormal, &normal);
	if (dot < 0.0f) return false;

	D3DXVec3Cross(&normal, &inter, &e1);
	D3DXVec3Normalize(&normal, &normal);
	dot = D3DXVec3Dot(&faceNormal, &normal);
	if (dot < 0.0f) return false;

	return true;
}

LPick::LPick()
{
	_vIntersection = TVector3(0, 0, 0);
}

LPick::~LPick()
{
}