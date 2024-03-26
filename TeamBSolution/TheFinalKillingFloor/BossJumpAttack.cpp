#include "BossJumpAttack.h"
#include "LGlobal.h"
#include "LFbxMgr.h"
#include "UIManager.h"

bool BossJumpAttack::Init()
{
    return true;
}

void BossJumpAttack::Process()
{
    m_pOwner->IsOnAir = true;

    if (m_pOwner->IsTakeDamage)
    {
        m_pOwner->SetTransition(Event::TAKEDAMAGE);
        return;
    }

    if (m_pOwner->m_TimerEnd)
    {
        m_pOwner->IsOnAir = false;
        m_pOwner->IsUseRush = false;
        m_pOwner->SetTransition(Event::ENDATTACK);
        return;
    }

    if (m_pOwner->IsMovable)
    {
        m_pOwner->JumpAttackMove(m_pOwner->m_PlayerPos);
    }

    m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Boss_Crawl.fbx");
}

void BossJumpAttack::Release()
{

}

BossJumpAttack::BossJumpAttack(Boss* parent) : NPCState(parent)
{

}

BossJumpAttack::~BossJumpAttack() {}