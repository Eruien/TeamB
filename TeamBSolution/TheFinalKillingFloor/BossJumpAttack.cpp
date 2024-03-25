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
    if (m_pOwner->IsMovable)
    {
        m_pOwner->JumpAttackMove(m_pOwner->m_PlayerPos);
    }

    m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Boss_JumpAttack.fbx");
}

void BossJumpAttack::Release()
{

}

BossJumpAttack::BossJumpAttack(Boss* parent) : NPCState(parent)
{

}

BossJumpAttack::~BossJumpAttack() {}