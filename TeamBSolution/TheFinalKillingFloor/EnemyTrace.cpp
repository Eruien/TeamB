#include "EnemyTrace.h"
#include "LGlobal.h"
#include "LFbxMgr.h"

bool EnemyTrace::Init()
{
    return true;
}

void EnemyTrace::Process()
{
    if (m_pOwner->IsDead)
    {
        m_pOwner->SetTransition(Event::FATALDAMAGE);
        return;
    }

    m_pOwner->Move(m_pOwner->m_PlayerPos);
    m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Zombie_Walk_Lock.fbx");
}

void EnemyTrace::Release()
{

}

EnemyTrace::EnemyTrace(LNPC* parent) : NPCState(parent)
{

}
EnemyTrace::~EnemyTrace() {}