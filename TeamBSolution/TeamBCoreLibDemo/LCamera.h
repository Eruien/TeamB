#pragma once
#include "LStd.h"

enum class CullResult {
    SPANNING,
    INSIDE,
    OUTSIDE
};

class xPlane
{
public:
    float fa, fb, fc, fd;
public:
    void Normalize()
    {
        float fMag = sqrt(fa * fa + fb * fb + fc * fc);
        fa = fa / fMag;
        fb = fb / fMag;
        fc = fc / fMag;
        fd = fd / fMag;
    }

    void   CreatePlane(TVector3 v0, TVector3 v1, TVector3 v2)
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

    void CreatePlane(TVector3 v0, TVector3 vNormal)
    {
        D3DXVec3Normalize(&vNormal, &vNormal);
        fa = vNormal.x;
        fb = vNormal.y;
        fc = vNormal.z;
        fd = -(fa * v0.x + fb * v0.y + fc * v0.z);
    }
};

class TFrustum
{
public:
    TVector3  m_vFrustum[8];
    xPlane    m_xPlane[6];
public:
    void Create(TMatrix matView, TMatrix matProj)
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
    
    CullResult CheckOBBInPlane(T_BOX& box)
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
};

class LCamera
{
public:
    TFrustum m_Frustum;
public:
    LVector m_vLook;
    LVector m_vUp;
    LVector m_vSide;
    float m_fSpeed = 10.0f;
public:
    LMatrix m_matView;
    LMatrix m_matOrthoProjection;
    LMatrix m_matProj;
    LVector m_vCameraPos;
    LVector m_vTargetPos;
    DWORD m_dwWindowWidth;
    DWORD m_dwWindowHeight;
public:
    LMatrix   CreateLookAt(LVector p, LVector t, LVector n = { 0,1,0 });
    LMatrix   CreatePerspectiveFov(float fFov, float fAspect, float fNear, float fFar);
    bool Frame2D();
    void UpdateVector();
public:
    virtual bool Init();
    virtual bool Frame();
    virtual bool Render();
    virtual bool Release();
public:
    void Create(LVector pos, LVector2 size);
public:
    virtual ~LCamera() {}
};

