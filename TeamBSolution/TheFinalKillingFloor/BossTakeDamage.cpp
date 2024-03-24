#include "BossTakeDamage.h"
#include "LGlobal.h"
#include "LFbxMgr.h"
#include "UIManager.h"

bool BossTakeDamage::Init()
{
    return true;
}

void BossTakeDamage::Process()
{
    m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Boss_Run.fbx");
}

void BossTakeDamage::Release()
{

}

BossTakeDamage::BossTakeDamage(Boss* parent) : NPCState(parent)
{

}

BossTakeDamage::~BossTakeDamage() {}