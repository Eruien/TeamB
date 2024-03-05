#include "LWeaponMgr.h"

void LWeaponMgr::Add(GunState key, std::shared_ptr<LWeapon> data)
{
	m_map.insert(std::make_pair(key, data));
}

LWeapon* LWeaponMgr::GetPtr(GunState key)
{
	auto iter = m_map.find(key);

	if (m_map.end() == iter)
	{
		return nullptr;
	}

	return iter->second.get();
}

bool LWeaponMgr::Get(GunState key, LWeapon& ret)
{
	auto iter = m_map.find(key);

	if (m_map.end() == iter)
	{
		return false;
	}

	ret = *(iter->second);

	return true;
}




