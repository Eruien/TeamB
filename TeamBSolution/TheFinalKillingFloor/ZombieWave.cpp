#include "ZombieWave.h"
#include "LFbxMgr.h"
#include "LPlayer.h"

void ZombieWave::SetZombie()
{
	m_WaveZombieCount = Wave1;
	m_ZombieModelList.resize(m_WaveZombieCount);

    for (int i = 0; i < m_WaveZombieCount; i++)
    {
        m_ZombieModelList[i] = new LNPC();
        m_ZombieModelList[i]->m_pModel = LFbxMgr::GetInstance().GetPtr(L"Zombie.fbx");
        m_ZombieModelList[i]->CreateBoneBuffer();
        m_ZombieModelList[i]->FSM(FSMType::ENEMY);

        m_ZombieModelList[i]->m_matControl._41 = GetRandomNumber();
        m_ZombieModelList[i]->m_matControl._43 = GetRandomNumber();
    }
    m_ZombieModelList[0]->ComputeOffset();
}

void ZombieWave::SpawnZombieWave(LPlayer* player)
{
    for (int i = 0; i < m_WaveZombieCount; i++)
    {
        m_ZombieModelList[i]->m_Player = player;
        m_ZombieModelList[i]->SetAnimationArrayTexture();
        m_ZombieModelList[i]->SetAnimationArraySRV();
        m_ZombieModelList[i]->CreateCurrentFrameBuffer();
    }
}

float ZombieWave::GetRandomNumber()
{
	return m_Distribution(m_Generator);
}

ZombieWave::ZombieWave()
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	m_Generator.seed(seed);
}