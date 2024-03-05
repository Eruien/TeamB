#include "LWeaponMgr.h"

void LWeaponMgr::Add(WeaponState key, std::shared_ptr<LWeapon> data)
{
	m_map.insert(std::make_pair(key, data));
}

LWeapon* LWeaponMgr::GetPtr(WeaponState key)
{
	auto iter = m_map.find(key);

	if (m_map.end() == iter)
	{
		return nullptr;
	}

	return iter->second.get();
}

bool LWeaponMgr::Get(WeaponState key, LWeapon& ret)
{
	auto iter = m_map.find(key);

	if (m_map.end() == iter)
	{
		return false;
	}

	ret = *(iter->second);

	return true;
}




