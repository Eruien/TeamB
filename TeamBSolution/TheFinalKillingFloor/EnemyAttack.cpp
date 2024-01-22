#include "EnemyAttack.h"
#include "LGlobal.h"
#include "LFbxMgr.h"

bool EnemyAttack::Init()
{
    return true;
}

void EnemyAttack::Process()
{
    if (m_pOwner->IsTakeDamage)
    {
        m_Timer = false;
        m_pOwner->SetTransition(Event::TAKEDAMAGE);
        return;
    }

    if (!m_pOwner->IsAttackRange)
    {
        m_Timer = false;
        m_pOwner->SetTransition(Event::PLAYEROUTATTACKRANGE);
        return;
    }

    m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Zombie_Attack_Anim.fbx");

    if (!m_Timer)
    {
        m_pOwner->m_TimerStart = true;
        m_Timer = true;
    }
}

void EnemyAttack::Release()
{

}

EnemyAttack::EnemyAttack(LNPC* parent) : NPCState(parent)
{

}
EnemyAttack::~EnemyAttack() {}