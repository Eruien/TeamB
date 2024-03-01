#include "ZombieWave.h"
#include "LFbxMgr.h"
#include "LPlayer.h"

void ZombieWave::SetZombie()
{
	
}

void ZombieWave::SpawnZombieWave(LPlayer* player)
{
 
}

float ZombieWave::GetRandomNumber()
{
	return m_Distribution(m_Generator);
}

ZombieWave::ZombieWave()
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	m_Generator.seed(seed);
    
    m_WaveZombieCountList.insert(std::make_pair(1, ZombieWave1));
    m_WaveZombieCountList.insert(std::make_pair(2, ZombieWave2));
    m_WaveZombieCountList.insert(std::make_pair(3, ZombieWave3));

    m_WaveTankCountList.insert(std::make_pair(1, 0));
    m_WaveTankCountList.insert(std::make_pair(2, TankWave2));
    m_WaveTankCountList.insert(std::make_pair(3, TankWave3));
}