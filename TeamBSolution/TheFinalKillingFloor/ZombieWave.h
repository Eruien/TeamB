#pragma once
#include "LNPC.h"
#include <random>
#include <chrono>

class ZombieWave
{
public:
	std::vector<LNPC*> m_ZombieModelList;
	int m_WaveZombieCount = 0;
	int m_CurrentZombieCount = 0;
	std::default_random_engine m_Generator;
	std::uniform_int_distribution<int> m_Distribution{ -1020, 1020 };
	std::map<int, int> m_WaveCountList;
	std::map<int, int> m_TankCountList;
	const int Wave1 = 5;
	const int Wave2 = 10;
	const int Wave3 = 15;
	const int TankWave2 = 1;
	const int TankWave3 = 2;
public:
	void SetZombie();
	void SpawnZombieWave(LPlayer* player);
	float GetRandomNumber();
public:
	ZombieWave();
};

