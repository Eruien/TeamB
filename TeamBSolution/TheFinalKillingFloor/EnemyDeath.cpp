#include "EnemyDeath.h"
#include "LGlobal.h"
#include "LFbxMgr.h"

bool EnemyDeath::Init()
{
    return true;
}

void EnemyDeath::Process()
{
    m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Zombie_Death.fbx");
}

void EnemyDeath::Release()
{

}

EnemyDeath::EnemyDeath(LNPC* parent) : NPCState(parent)
{

}
EnemyDeath::~EnemyDeath() {}