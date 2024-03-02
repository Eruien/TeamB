#pragma once
#include "LNPC.h"
#include "LGlobal.h"
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
	std::vector<std::shared_ptr<LNPC>> m_ZombieModelList;
	std::vector<std::shared_ptr<LNPC>> m_TankList;
	int m_CurrentWave = 1;
public:
	void SetZombie();
	void SpawnZombieWave(LPlayer* player);
	float GetRandomNumber();
	void CollisionCheckOBB(std::vector<shared_ptr<LModel>>& collisionObject, std::vector<LNPC*>& zombieModelList);

	template <typename T, typename U>
	void CollisionCheckByDistance(std::vector<std::shared_ptr<T>>& collisionObject, std::vector<std::shared_ptr<U>>& zombieModelList)
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
                float offsetX = zombieModelList[i]->m_OBBBox.m_Box.vCenter.x - zombieModelList[j]->m_OBBBox.m_Box.vCenter.x;
                float offsetY = zombieModelList[i]->m_OBBBox.m_Box.vCenter.y - zombieModelList[j]->m_OBBBox.m_Box.vCenter.y;
                float offsetZ = zombieModelList[i]->m_OBBBox.m_Box.vCenter.z - zombieModelList[j]->m_OBBBox.m_Box.vCenter.z;
                TVector3 length = { offsetX, offsetY, offsetZ };
                float distance = length.Length();
                TVector3 range = zombieModelList[i]->m_OBBBox.m_Box.vMax - zombieModelList[i]->m_OBBBox.m_Box.vMin;
                float r = range.Length();
                r *= 0.5f; // ���񳢸��� �� ������� ������
                if (distance <= r)
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

            }
            //// Player <-> zombie
            //if (LGlobal::g_PlayerModel->m_OBBBox.CollisionCheckOBB(&zombieModelList[i]->m_OBBBox))
            //{
            //    float offsetX = LGlobal::g_PlayerModel->m_OBBBox.m_Box.vCenter.x - zombieModelList[i]->m_OBBBox.m_Box.vCenter.x;
            //    float offsetZ = LGlobal::g_PlayerModel->m_OBBBox.m_Box.vCenter.z - zombieModelList[i]->m_OBBBox.m_Box.vCenter.z;

            //    LGlobal::g_PlayerModel->m_matControl._41 += offsetX * 0.1;
            //    LGlobal::g_PlayerModel->m_matControl._43 += offsetZ * 0.1;
            //}

            // zombie <-> tree
            for (auto& tree : collisionObject)
            {
                TVector3 length = { tree->m_matControl._41, zombieModelList[i]->m_matControl._42, tree->m_matControl._43 };
                length -= zombieModelList[i]->m_OBBBox.m_Box.vCenter;
                float distance = length.Length();
                if (distance <= 30)
                {
                    float offsetX = zombieModelList[i]->m_OBBBox.m_Box.vCenter.x - tree->m_matControl._41;
                    float offsetZ = zombieModelList[i]->m_OBBBox.m_Box.vCenter.z - tree->m_matControl._43;

                    TVector3 vNormal = { offsetX, zombieModelList[i]->m_matControl._42, offsetZ };
                    vNormal.Normalize();
                    vNormal *= (30 - distance);
                    zombieModelList[i]->m_matControl._41 += vNormal.x;
                    zombieModelList[i]->m_matControl._43 += vNormal.z;
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

	void CollisionBoxRender();
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	ZombieWave();
};

