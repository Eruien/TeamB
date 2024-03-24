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
    m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Boss_Run.fbx");
}

void BossJumpAttack::Release()
{

}

BossJumpAttack::BossJumpAttack(Boss* parent) : NPCState(parent)
{

}

BossJumpAttack::~BossJumpAttack() {}