#include "LFrustum.h"

void xPlane::Normalize()
{
    float fMag = sqrt(fa * fa + fb * fb + fc * fc);
    fa = fa / fMag;
    fb = fb / fMag;
    fc = fc / fMag;
    fd = fd / fMag;
}

void xPlane::CreatePlane(TVector3 v0, TVector3 v1, TVector3 v2)
{
    TVector3 vE0 = v1 - v0;
    TVector3 vE1 = v2 - v0;
    TVector3 vNormal;
    D3DXVec3Cross(&vNormal, &vE0, &vE1);
    D3DXVec3Normalize(&vNormal, &vNormal);
    fa = vNormal.x;
    fb = vNormal.y;
    fc = vNormal.z;
    fd = -(fa * v0.x + fb * v0.y + fc * v0.z);
}

void xPlane::CreatePlane(TVector3 v0, TVector3 vNormal)
{
    D3DXVec3Normalize(&vNormal, &vNormal);
    fa = vNormal.x;
    fb = vNormal.y;
    fc = vNormal.z;
    fd = -(fa * v0.x + fb * v0.y + fc * v0.z);
}

void LFrustum::Create(TMatrix matView, TMatrix matProj)
{
    TMatrix mat = matView * matProj;
    D3DXMatrixInverse(&mat, NULL, &mat);
    // 원단면
    // 5   6
    // 4   7
    // 근단면
    // 1   2
    // 0   3
    m_vFrustum[0] = TVector3(-1.0f, -1.0f, 0.0f);
    m_vFrustum[1] = TVector3(-1.0f, 1.0f, 0.0f);
    m_vFrustum[2] = TVector3(1.0f, 1.0f, 0.0f);
    m_vFrustum[3] = TVector3(1.0f, -1.0f, 0.0f);

    m_vFrustum[4] = TVector3(-1.0f, -1.0f, 1.0f);
    m_vFrustum[5] = TVector3(-1.0f, 1.0f, 1.0f);
    m_vFrustum[6] = TVector3(1.0f, 1.0f, 1.0f);
    m_vFrustum[7] = TVector3(1.0f, -1.0f, 1.0f);

    for (int i = 0; i < 8; i++)
    {
        D3DXVec3TransformCoord(&m_vFrustum[i],
            &m_vFrustum[i], &mat);
    }

    m_xPlane[0].CreatePlane(m_vFrustum[5], m_vFrustum[0], m_vFrustum[1]);	// 좌 평면(left)
    m_xPlane[1].CreatePlane(m_vFrustum[3], m_vFrustum[6], m_vFrustum[2]);	// 우 평면(right)
    m_xPlane[2].CreatePlane(m_vFrustum[5], m_vFrustum[2], m_vFrustum[6]);	// 상 평면(top)
    m_xPlane[3].CreatePlane(m_vFrustum[0], m_vFrustum[7], m_vFrustum[3]);	// 하 평면(bottom)
    m_xPlane[4].CreatePlane(m_vFrustum[0], m_vFrustum[2], m_vFrustum[1]);	// 근 평면(near)
    m_xPlane[5].CreatePlane(m_vFrustum[6], m_vFrustum[4], m_vFrustum[5]);	// 원 평면(far)
}

CullResult LFrustum::CheckOBBInPlane(T_BOX& box)
{
    float fDistance = 0.0f;
    float fPlaneToCenter = 0.0f;
    int insideCount = 0;
    TVector3 vDir;

    for (int iplane = 0; iplane < 6; iplane++)
    {
        vDir = box.vAxis[0] * box.fExtent[0];
        fDistance = fabs(m_xPlane[iplane].fa * vDir.x +
            m_xPlane[iplane].fb * vDir.y +
            m_xPlane[iplane].fc * vDir.z);

        vDir = box.vAxis[1] * box.fExtent[1];
        fDistance += fabs(m_xPlane[iplane].fa * vDir.x +
            m_xPlane[iplane].fb * vDir.y +
            m_xPlane[iplane].fc * vDir.z);

        vDir = box.vAxis[2] * box.fExtent[2];
        fDistance += fabs(m_xPlane[iplane].fa * vDir.x +
            m_xPlane[iplane].fb * vDir.y +
            m_xPlane[iplane].fc * vDir.z);

        fPlaneToCenter = m_xPlane[iplane].fa * box.vCenter.x +
            m_xPlane[iplane].fb * box.vCenter.y +
            m_xPlane[iplane].fc * box.vCenter.z +
            m_xPlane[iplane].fd;

        if (fPlaneToCenter < -fDistance)
        {
            return CullResult::OUTSIDE;
        }
        else if (fPlaneToCenter <= fDistance)
        {

        }
        else
        {
            insideCount++;
        }
    }

    if (insideCount == 6) {
        return CullResult::INSIDE;
    }

    return CullResult::SPANNING;
}