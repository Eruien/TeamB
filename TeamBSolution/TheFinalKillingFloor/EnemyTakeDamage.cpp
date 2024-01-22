#include "EnemyTakeDamage.h"
#include "LGlobal.h"
#include "LFbxMgr.h"

bool EnemyTakeDamage::Init()
{
    return true;
}

void EnemyTakeDamage::Process()
{
    if (m_pOwner->IsDead)
    {
        m_pOwner->SetTransition(Event::FATALDAMAGE);
        return;
    }

    m_pOwner->Move(m_pOwner->m_PlayerPos);
    m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Zombie_Walk_Lock.fbx");
}

void EnemyTakeDamage::Release()
{

}

EnemyTakeDamage::EnemyTakeDamage(LNPC* parent) : NPCState(parent)
{

}
EnemyTakeDamage::~EnemyTakeDamage() {}