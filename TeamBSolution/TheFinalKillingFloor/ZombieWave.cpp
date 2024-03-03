#include "ZombieWave.h"
#include "LFbxMgr.h"
#include "LPlayer.h"
#include "LGlobal.h"

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

void ZombieWave::CollisionCheckOBB(std::vector<shared_ptr<LModel>>& collisionObject, std::vector<LNPC*>& zombieModelList)
{
    for (int i = 0; i < zombieModelList.size(); i++)
    {
        // collision check
        if (LGlobal::g_PlayerModel->m_OBBBox.CollisionCheckOBB(&zombieModelList[i]->m_OBBBox)
            || LGlobal::g_PlayerModel->m_OBBBox.CollisionCheckOBB(&zombieModelList[i]->m_OBBBoxRightHand))
        {
            LGlobal::g_PlayerModel->IsTakeDamage = true;
        }

        for (int j = i + 1; j < zombieModelList.size(); j++)
        {
            if (zombieModelList[i]->m_OBBBox.CollisionCheckOBB(&zombieModelList[j]->m_OBBBox))
            {
                float offsetX = zombieModelList[i]->m_OBBBox.m_Box.vCenter.x - zombieModelList[i]->m_OBBBox.m_Box.vCenter.x;
                float offsetZ = zombieModelList[i]->m_OBBBox.m_Box.vCenter.z - zombieModelList[i]->m_OBBBox.m_Box.vCenter.z;

                zombieModelList[i]->m_matControl._41 += offsetX * 0.1;
                zombieModelList[i]->m_matControl._43 += offsetZ * 0.1;
            }

        }

        // Player <-> zombie
        if (LGlobal::g_PlayerModel->m_OBBBox.CollisionCheckOBB(&zombieModelList[i]->m_OBBBox))
        {
            float offsetX = LGlobal::g_PlayerModel->m_OBBBox.m_Box.vCenter.x - zombieModelList[i]->m_OBBBox.m_Box.vCenter.x;
            float offsetZ = LGlobal::g_PlayerModel->m_OBBBox.m_Box.vCenter.z - zombieModelList[i]->m_OBBBox.m_Box.vCenter.z;

            LGlobal::g_PlayerModel->m_matControl._41 += offsetX * 0.1;
            LGlobal::g_PlayerModel->m_matControl._43 += offsetZ * 0.1;
        }

        // zombie <-> tree
        for (auto& tree : collisionObject)
        {
            TVector3 length = { tree->m_matControl._41, tree->m_matControl._42, tree->m_matControl._43 };
            length -= zombieModelList[i]->m_OBBBox.m_Box.vCenter;
            float distance = length.Length();
            if (distance <= 27)
            {
                float offsetX = zombieModelList[i]->m_OBBBox.m_Box.vCenter.x - tree->m_matControl._41;
                float offsetZ = zombieModelList[i]->m_OBBBox.m_Box.vCenter.z - tree->m_matControl._43;

                zombieModelList[i]->m_matControl._41 += offsetX * 0.1;
                zombieModelList[i]->m_matControl._43 += offsetZ * 0.1;
            }
        }
    }

    for (int i = 0; i < zombieModelList.size(); i++)
    {
        zombieModelList[i]->m_OBBBox.Frame();
        zombieModelList[i]->m_OBBBox.CreateOBBBox(zombieModelList[i]->m_SettingBox.fExtent[0], zombieModelList[i]->m_SettingBox.fExtent[1], zombieModelList[i]->m_SettingBox.fExtent[2],
            { zombieModelList[i]->m_OBBBox.m_matWorld._41, zombieModelList[i]->m_OBBBox.m_matWorld._42, zombieModelList[i]->m_OBBBox.m_matWorld._43 },
            zombieModelList[i]->m_SettingBox.vAxis[0], zombieModelList[i]->m_SettingBox.vAxis[1], zombieModelList[i]->m_SettingBox.vAxis[2]);

        zombieModelList[i]->m_OBBBoxRightHand.Frame();
        zombieModelList[i]->m_OBBBoxRightHand.CreateOBBBox(zombieModelList[i]->m_SettingBoxRightHand.fExtent[0], zombieModelList[i]->m_SettingBoxRightHand.fExtent[1], zombieModelList[i]->m_SettingBoxRightHand.fExtent[2],
            { zombieModelList[i]->m_OBBBoxRightHand.m_matWorld._41, zombieModelList[i]->m_OBBBoxRightHand.m_matWorld._42, zombieModelList[i]->m_OBBBoxRightHand.m_matWorld._43 },
            zombieModelList[i]->m_SettingBoxRightHand.vAxis[0], zombieModelList[i]->m_SettingBoxRightHand.vAxis[1], zombieModelList[i]->m_SettingBoxRightHand.vAxis[2]);
    }
}
void ZombieWave::CollisionCheckWithObstacle(std::vector<std::shared_ptr<LModel>>& collisionObject)
{
    // npc <-> tree
    for (auto& tree : collisionObject)
    {
        // 모든 키에 대해 충돌 체크
        for (auto& npcPair : m_EnemyMap)
        {
            for (auto& npc : npcPair.second)  // npcPair.second는 해당 키의 값을 가져옵니다.
            {
                TVector3 length = { tree->m_matControl._41, npc->m_matControl._42, tree->m_matControl._43 };
                length -= npc->m_OBBBox.m_Box.vCenter;
                float distance = length.Length();

                if (distance <= 30)
                {
                    float offsetX = npc->m_OBBBox.m_Box.vCenter.x - tree->m_matControl._41;
                    float offsetZ = npc->m_OBBBox.m_Box.vCenter.z - tree->m_matControl._43;

                    TVector3 vNormal = { offsetX, npc->m_matControl._42, offsetZ };
                    vNormal.Normalize();
                    vNormal *= (30 - distance);
                    npc->m_matControl._41 += vNormal.x;
                    npc->m_matControl._43 += vNormal.z;
                }
            }
        }
    }
}
void ZombieWave::CollisionCheckInNpc()
{
    for (auto& npcPair : m_EnemyMap)
    {
        auto& zombieModelList = npcPair.second;
        for (int i = 0; i < zombieModelList.size(); ++i)
        {
            if (LGlobal::g_PlayerModel->m_OBBBox.CollisionCheckOBB(&zombieModelList[i]->m_OBBBox)
                || LGlobal::g_PlayerModel->m_OBBBox.CollisionCheckOBB(&zombieModelList[i]->m_OBBBoxRightHand))
            {
                LGlobal::g_PlayerModel->IsTakeDamage = true;
            }

            for (int i = 0; i < zombieModelList.size(); i++)
            {
                zombieModelList[i]->m_OBBBox.Frame();
                zombieModelList[i]->m_OBBBox.CreateOBBBox(zombieModelList[i]->m_SettingBox.fExtent[0], zombieModelList[i]->m_SettingBox.fExtent[1], zombieModelList[i]->m_SettingBox.fExtent[2],
                    { zombieModelList[i]->m_OBBBox.m_matWorld._41, zombieModelList[i]->m_OBBBox.m_matWorld._42, zombieModelList[i]->m_OBBBox.m_matWorld._43 },
                    zombieModelList[i]->m_SettingBox.vAxis[0], zombieModelList[i]->m_SettingBox.vAxis[1], zombieModelList[i]->m_SettingBox.vAxis[2]);

                zombieModelList[i]->m_OBBBoxRightHand.Frame();
                zombieModelList[i]->m_OBBBoxRightHand.CreateOBBBox(zombieModelList[i]->m_SettingBoxRightHand.fExtent[0], zombieModelList[i]->m_SettingBoxRightHand.fExtent[1], zombieModelList[i]->m_SettingBoxRightHand.fExtent[2],
                    { zombieModelList[i]->m_OBBBoxRightHand.m_matWorld._41, zombieModelList[i]->m_OBBBoxRightHand.m_matWorld._42, zombieModelList[i]->m_OBBBoxRightHand.m_matWorld._43 },
                    zombieModelList[i]->m_SettingBoxRightHand.vAxis[0], zombieModelList[i]->m_SettingBoxRightHand.vAxis[1], zombieModelList[i]->m_SettingBoxRightHand.vAxis[2]);
            }

            for (int j = i + 1; j < zombieModelList.size(); ++j)
            {


                float offsetX = zombieModelList[i]->m_OBBBox.m_Box.vCenter.x - zombieModelList[j]->m_OBBBox.m_Box.vCenter.x;
                float offsetY = zombieModelList[i]->m_OBBBox.m_Box.vCenter.y - zombieModelList[j]->m_OBBBox.m_Box.vCenter.y;
                float offsetZ = zombieModelList[i]->m_OBBBox.m_Box.vCenter.z - zombieModelList[j]->m_OBBBox.m_Box.vCenter.z;
                TVector3 length = { offsetX, offsetY, offsetZ };
                float distance = length.Length();
                TVector3 range = zombieModelList[i]->m_OBBBox.m_Box.vMax - zombieModelList[i]->m_OBBBox.m_Box.vMin;
                float r = range.Length();
                r *= 0.7f; // 좀비끼리는 더 가까워도 괜찮음
                if (distance <= r)
                {
                    if (!zombieModelList[i]->IsRush &&
                        !zombieModelList[j]->IsRush)
                    {
                        if (zombieModelList[i]->m_CurrentState != State::ENEMYATTACK)
                        {
                            TVector3 vNormal = { offsetX, zombieModelList[i]->m_matControl._42, offsetZ };
                            vNormal.Normalize();
                            vNormal *= (r - distance);
                            zombieModelList[i]->m_matControl._41 += vNormal.x;
                            zombieModelList[i]->m_matControl._43 += vNormal.z;
                        }
                        else
                        {
                            TVector3 vNormal = { offsetX, zombieModelList[i]->m_matControl._42, offsetZ };
                            vNormal.Normalize();
                            vNormal *= (r - distance);
                            zombieModelList[j]->m_matControl._41 -= vNormal.x;
                            zombieModelList[j]->m_matControl._43 -= vNormal.z;
                        }
                    }
                    else
                    {
                        if (zombieModelList[i]->IsRush)
                        {
							TVector3 vNormal = { offsetX, zombieModelList[i]->m_matControl._42, offsetZ };
							vNormal.Normalize();
							vNormal *= (r - distance);
							zombieModelList[j]->m_matControl._41 -= vNormal.x;
							zombieModelList[j]->m_matControl._43 -= vNormal.z;
						}
                        else
                        {
                            TVector3 vNormal = { offsetX, zombieModelList[i]->m_matControl._42, offsetZ };
                            vNormal.Normalize();
                            vNormal *= (r - distance);
                            zombieModelList[i]->m_matControl._41 += vNormal.x;
                            zombieModelList[i]->m_matControl._43 += vNormal.z;
                        }
                    }
                }
            }
        }
    }
}

void ZombieWave::CollisionBoxRender()
{
    
    for (int i = 0; i < m_EnemyMap["Zombie"].size(); i++)
    {
        TMatrix zombieTranslation;
        zombieTranslation.Translation(TVector3(m_EnemyMap["Zombie"][i]->m_matControl._41, m_EnemyMap["Zombie"][i]->m_matControl._42 + m_EnemyMap["Zombie"][i]->m_SettingBox.vCenter.y, m_EnemyMap["Zombie"][i]->m_matControl._43));
        m_EnemyMap["Zombie"][i]->m_OBBBox.SetMatrix(&zombieTranslation, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
        //m_ZombieModelList[i]->m_OBBBox.Render();
        m_EnemyMap["Zombie"][i]->Render();

        TMatrix zombieRightHandSocket;
        TMatrix matRightHand;
        if (m_EnemyMap["Zombie"][i]->m_pActionModel != nullptr)
        {
            if (m_EnemyMap["Zombie"][i]->m_pActionModel->m_iEndFrame >= int(m_EnemyMap["Zombie"][i]->m_fCurrentAnimTime))
            {
                int currentFrame = max(m_EnemyMap["Zombie"][i]->m_fCurrentAnimTime - m_EnemyMap["Zombie"][i]->m_pActionModel->m_iStartFrame, 0);
                zombieRightHandSocket = m_EnemyMap["Zombie"][i]->m_pActionModel->m_NameMatrixMap[int(currentFrame)][L"RightHand"];
            }
        }

        matRightHand = zombieRightHandSocket * m_EnemyMap["Zombie"][i]->m_matControl;
        m_EnemyMap["Zombie"][i]->m_OBBBoxRightHand.SetMatrix(&matRightHand, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
        //m_EnemyMap["Zombie"][i]->m_OBBBoxRightHand.Render();
    }

    for (int i = 0; i < m_EnemyMap["Tank"].size(); i++)
    {
        TMatrix zombieTranslation;
        zombieTranslation.Translation(TVector3(m_EnemyMap["Tank"][i]->m_matControl._41, m_EnemyMap["Tank"][i]->m_matControl._42 + m_EnemyMap["Tank"][i]->m_SettingBox.vCenter.y, m_EnemyMap["Tank"][i]->m_matControl._43));
        m_EnemyMap["Tank"][i]->m_OBBBox.SetMatrix(&zombieTranslation, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
        //m_TankList[i]->m_OBBBox.Render();
        m_EnemyMap["Tank"][i]->Render();

        TMatrix zombieRightHandSocket;
        TMatrix matRightHand;
        if (m_EnemyMap["Tank"][i]->m_pActionModel != nullptr)
        {
            if (m_EnemyMap["Tank"][i]->m_pActionModel->m_iEndFrame >= int(m_EnemyMap["Tank"][i]->m_fCurrentAnimTime))
            {
                int currentFrame = max(m_EnemyMap["Tank"][i]->m_fCurrentAnimTime - m_EnemyMap["Tank"][i]->m_pActionModel->m_iStartFrame, 0);
                zombieRightHandSocket = m_EnemyMap["Tank"][i]->m_pActionModel->m_NameMatrixMap[int(currentFrame)][L"RightHand"];
            }
        }

        matRightHand = zombieRightHandSocket * m_EnemyMap["Tank"][i]->m_matControl;
        m_EnemyMap["Tank"][i]->m_OBBBoxRightHand.SetMatrix(&matRightHand, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
        //m_TankList[i]->m_OBBBoxRightHand.Render();
    }
}


bool ZombieWave::Init()
{
    return true;
}

bool ZombieWave::Frame()
{
    for (int i = 0; i < m_EnemyMap["Zombie"].size(); i++)
    {
        m_EnemyMap["Zombie"][i]->Process();
        m_EnemyMap["Zombie"][i]->Frame();
    }

    for (int i = 0; i < m_EnemyMap["Tank"].size(); i++)
    {
        m_EnemyMap["Tank"][i]->Process();
        m_EnemyMap["Tank"][i]->Frame();
    }

    return true;
}

bool ZombieWave::Render()
{
    for (auto& zombie : m_EnemyMap["Zombie"])
    {
        zombie->AniRender();
    }

    for (auto& tank : m_EnemyMap["Tank"])
    {
        tank->Render();
    }

    return true;
}

bool ZombieWave::Release()
{
    return true;
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