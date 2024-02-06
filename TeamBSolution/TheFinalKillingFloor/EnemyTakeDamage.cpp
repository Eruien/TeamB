#include "EnemyTakeDamage.h"
#include "LGlobal.h"
#include "LFbxMgr.h"

bool EnemyTakeDamage::Init()
{
    return true;
}

void EnemyTakeDamage::Process()
{
    m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Zombie_TakeDamage.fbx");

    if (!m_Timer)
    {
        m_pOwner->m_HP -= 20.0f;
        m_pOwner->IsTakeDamage = false;
        m_pOwner->m_TimerStart = true;
        m_Timer = true;
    }

    if (m_pOwner->IsTakeDamage)
    {
        m_pOwner->m_HP -= 20.0f;
        m_pOwner->IsTakeDamage = false;
    }

    if (m_pOwner->IsDead)
    {
        m_pOwner->SetTransition(Event::FATALDAMAGE);
        return;
    }

    if (m_pOwner->m_TimerEnd)
    {
        m_Timer = false;
        m_pOwner->SetTransition(Event::RECOVERYDAMAGE);
        return;
    }
}

void EnemyTakeDamage::Release()
{

}

EnemyTakeDamage::EnemyTakeDamage(LNPC* parent) : NPCState(parent)
{

}
EnemyTakeDamage::~EnemyTakeDamage() {}