#pragma once
#include "LNPC.h"
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
	int m_CurrentWave = 1;
public:
	void SetZombie();
	void SpawnZombieWave(LPlayer* player);
	float GetRandomNumber();
public:
	ZombieWave();
};

