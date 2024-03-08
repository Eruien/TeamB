#include "LWeaponMgr.h"
#include "LGlobal.h"
#include "LPlayer.h"
#include "UIManager.h"
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

void LWeaponMgr::UpgradeMagazine(WeaponState weapon)
{
	m_map[weapon]->m_GunSpec.MagazineLevel += 1;
	m_map[weapon]->m_GunSpec.TotalAmmo = m_map[weapon]->m_GunSpec.defaultTotalAmmo+(m_map[weapon]->m_GunSpec.defaultTotalAmmo * 0.1 * m_map[weapon]->m_GunSpec.MagazineLevel);
	
	if (LGlobal::g_PlayerModel->m_CurrentGun == WeaponState::PISTOL)
	{
		UIManager::GetInstance().GetUIObject(L"T_Ammo")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(m_map[WeaponState::PISTOL]->m_GunSpec.TotalAmmo);
	}
	else if (LGlobal::g_PlayerModel->m_CurrentGun == WeaponState::ASSAULTRIFLE)
	{
		UIManager::GetInstance().GetUIObject(L"T_Ammo")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(m_map[WeaponState::ASSAULTRIFLE]->m_GunSpec.TotalAmmo);
	}
	else if (LGlobal::g_PlayerModel->m_CurrentGun == WeaponState::SHOTGUN)
	{
		UIManager::GetInstance().GetUIObject(L"T_Ammo")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(m_map[WeaponState::SHOTGUN]->m_GunSpec.TotalAmmo);
	}

	if (weapon == WeaponState::PISTOL)
	{
		UIManager::GetInstance().GetUIObject(L"T_Gun1_MAGAZINE")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring(m_map[weapon]->m_GunSpec.TotalAmmo));
	}
	else if (weapon == WeaponState::ASSAULTRIFLE)
	{
		UIManager::GetInstance().GetUIObject(L"T_Gun2_MAGAZINE")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring(m_map[weapon]->m_GunSpec.TotalAmmo));
	}
	else if (weapon == WeaponState::SHOTGUN)
	{
		UIManager::GetInstance().GetUIObject(L"T_Gun3_MAGAZINE")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring(m_map[weapon]->m_GunSpec.TotalAmmo));
	}
}





