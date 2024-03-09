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

void LWeaponMgr::UpgradeDamage(WeaponState weapon)
{

	if (LGlobal::g_PlayerModel->m_Money < 100)
		return;
	LGlobal::g_PlayerModel->m_Money -= 100;

	m_map[weapon]->m_GunSpec.DamgeLevel += 1;
	m_map[weapon]->m_GunSpec.Damage = m_map[weapon]->m_GunSpec.defaultDamage + (m_map[weapon]->m_GunSpec.defaultDamage * 0.1 * m_map[weapon]->m_GunSpec.DamgeLevel);

	if (weapon == WeaponState::PISTOL)
	{
		UIManager::GetInstance().GetUIObject(L"T_Gun1_DAMAGE")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring((int)(m_map[weapon]->m_GunSpec.Damage)));
	}
	else if (weapon == WeaponState::ASSAULTRIFLE)
	{
		UIManager::GetInstance().GetUIObject(L"T_Gun2_DAMAGE")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring((int)(m_map[weapon]->m_GunSpec.Damage)));
	}
	else if (weapon == WeaponState::SHOTGUN)
	{
		UIManager::GetInstance().GetUIObject(L"T_Gun3_DAMAGE")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring((int)(m_map[weapon]->m_GunSpec.Damage)));
	}
	UIManager::GetInstance().GetUIObject(L"Money")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(LGlobal::g_PlayerModel->m_Money);
	UIManager::GetInstance().GetUIObject(L"Shop_Money")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(LGlobal::g_PlayerModel->m_Money);
}

void LWeaponMgr::UpgradeRPM(WeaponState weapon)
{
	if (LGlobal::g_PlayerModel->m_Money < 100)
		return;
	LGlobal::g_PlayerModel->m_Money -= 100;

	if (m_map[weapon]->m_GunSpec.RPMLevel == 8)
	{
		if (weapon == WeaponState::PISTOL)
			UIManager::GetInstance().GetUIObject(L"Gun1_RPM_price")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(L"MAX");
		else if (weapon == WeaponState::ASSAULTRIFLE)
			UIManager::GetInstance().GetUIObject(L"Gun2_RPM_price")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(L"MAX");
		else if (weapon == WeaponState::SHOTGUN)
			UIManager::GetInstance().GetUIObject(L"Gun3_RPM_price")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(L"MAX");
		

		//È¯ºÒ
		LGlobal::g_PlayerModel->m_Money += 100;
		return;
	}
	m_map[weapon]->m_GunSpec.RPMLevel += 1;
	m_map[weapon]->m_GunSpec.ShootDelay =   m_map[weapon]->m_GunSpec.defaultShootDelay-((0.1*m_map[weapon]->m_GunSpec.RPMLevel* m_map[weapon]->m_GunSpec.defaultShootDelay));

	if (weapon == WeaponState::PISTOL)
	{
		UIManager::GetInstance().GetUIObject(L"T_Gun1_RPM")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring((int)(60 / m_map[weapon]->m_GunSpec.ShootDelay)));
	}
	else if (weapon == WeaponState::ASSAULTRIFLE)
	{
		UIManager::GetInstance().GetUIObject(L"T_Gun2_RPM")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring((int)(60 / m_map[weapon]->m_GunSpec.ShootDelay)));
	}
	else if (weapon == WeaponState::SHOTGUN)
	{
		UIManager::GetInstance().GetUIObject(L"T_Gun3_RPM")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring((int)(60 / m_map[weapon]->m_GunSpec.ShootDelay)));
	}


	UIManager::GetInstance().GetUIObject(L"Money")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(LGlobal::g_PlayerModel->m_Money);
	UIManager::GetInstance().GetUIObject(L"Shop_Money")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(LGlobal::g_PlayerModel->m_Money);
}

void LWeaponMgr::UpgradeMagazine(WeaponState weapon)
{
	if (LGlobal::g_PlayerModel->m_Money < 100)
		return;
	LGlobal::g_PlayerModel->m_Money -= 100;

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
		if (LWeaponMgr::GetInstance().m_map[WeaponState::SHOTGUN]->m_GunSpec.TotalAmmo < 10)
			UIManager::GetInstance().GetUIObject(L"T_Gun3_MAGAZINE")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring(LWeaponMgr::GetInstance().m_map[WeaponState::SHOTGUN]->m_GunSpec.TotalAmmo).insert(0, L"0"));
		else
			UIManager::GetInstance().GetUIObject(L"T_Gun3_MAGAZINE")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring(LWeaponMgr::GetInstance().m_map[WeaponState::SHOTGUN]->m_GunSpec.TotalAmmo));
	}


	UIManager::GetInstance().GetUIObject(L"Money")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(LGlobal::g_PlayerModel->m_Money);
	UIManager::GetInstance().GetUIObject(L"Shop_Money")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(LGlobal::g_PlayerModel->m_Money);
}





