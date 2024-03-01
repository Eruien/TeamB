#pragma once
#include "LNPC.h"
#include "Tank.h"
#include <random>
#include <chrono>

class ZombieWave
{
public:
	std::default_random_engine m_Generator;
	std::uniform_int_distribution<int> m_Distribution{ -1020, 1020 };
	std::map<int, int> m_WaveZombieCountList;
	std::map<int, int> m_WaveTankCountList;
	const int ZombieWave1 = 5;
	const int ZombieWave2 = 10;
	const int ZombieWave3 = 15;
	const int TankWave2 = 1;
	const int TankWave3 = 2;
public:
	std::vector<LNPC*> m_ZombieModelList;
	std::vector<Tank*> m_TankList;
	int m_CurrentWave = 1;
public:
	void SetZombie();
	void SpawnZombieWave(LPlayer* player);
	float GetRandomNumber();
	void CollisionCheckOBB(std::vector<shared_ptr<LModel>>& collisionObject, std::vector<LNPC*>& zombieModelList);
	void CollisionCheckOBB(std::vector<shared_ptr<LModel>>& collisionObject, std::vector<Tank*>& zombieModelList);
	void CollisionCheckByDistance(std::vector<shared_ptr<LModel>>& collisionObject, std::vector<LNPC*>& zombieModelList);
	void CollisionCheckByDistance(std::vector<shared_ptr<LModel>>& collisionObject, std::vector<Tank*>& zombieModelList);
	void CollisionBoxRender();
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	ZombieWave();
};

