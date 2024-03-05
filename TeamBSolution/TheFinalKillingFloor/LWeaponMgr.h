#pragma once
#include "LModel.h"

struct GunSpec
{
	float ShootDelay = 0.0f;
	float Damage = 0.0f;
	int TotalAmmo = 0;
	int CurrentAmmo = 0;
};

class LWeapon
{
public:
	std::shared_ptr<LModel> m_WeaponModel = nullptr;
public:
	GunSpec m_GunSpec;
};

class LWeaponMgr
{
	using LList = std::map<GunState, std::shared_ptr<LWeapon>>;
public:
	static LWeaponMgr& GetInstance()
	{
		static LWeaponMgr input;
		return input;
	}
	LList m_map;
public:
	void Add(GunState key, std::shared_ptr<LWeapon> data);
public:
	LWeapon* GetPtr(GunState key);
	bool Get(GunState key, LWeapon& ret);
public:
	virtual ~LWeaponMgr() {};
};

