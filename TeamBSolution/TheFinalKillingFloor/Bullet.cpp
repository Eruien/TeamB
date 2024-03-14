#include "Bullet.h"


bool Bullet::Frame() 
{
	LModel::Frame();
	if (bTarget)
	{
		if (target->IsDead)
			bTarget = false;
		TVector3 targetDir = target->GetPosition();
		targetDir.y += target->m_OBBBox.fTall * 0.8f;
		TVector3 addDir = targetDir - this->GetPosition();
		float dist = 50 / addDir.Length();
		addDir.Normalize();
		addDir *= dist;
		m_Forward += addDir;
		m_Forward.Normalize();
	}
	m_matControl._41 += m_Forward.x * 15.f;
	m_matControl._42 += m_Forward.y * 15.f;// -LGlobal::g_fSPF * 30.f;
	m_matControl._43 += m_Forward.z * 15.f;
	return true;
}