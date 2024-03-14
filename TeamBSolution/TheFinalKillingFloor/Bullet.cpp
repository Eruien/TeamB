#include "Bullet.h"


bool Bullet::Frame() 
{
	LModel::Frame();
	if (bTarget)
	{
		if (target->m_HP < 0)
			bTarget = false;
		TVector3 targetDir = target->GetPosition();
		targetDir.y += target->m_OBBBox.fTall * 0.8f;
		TVector3 addDir = targetDir - this->GetPosition();
		float dist = 100 / addDir.Length();
		addDir.Normalize();
		addDir *= dist;
		m_Forward += addDir;
		m_Forward.Normalize();
	}
	m_matControl._41 += m_Forward.x * 20.f;
	m_matControl._42 += m_Forward.y * 20.f;
	m_matControl._43 += m_Forward.z * 20.f;
	return true;
}