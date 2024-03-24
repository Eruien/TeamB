#include "BossSwiping.h"
#include "LGlobal.h"
#include "LFbxMgr.h"
#include "UIManager.h"

bool BossSwiping::Init()
{
    return true;
}

void BossSwiping::Process()
{
    m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Boss_Run.fbx");
}

void BossSwiping::Release()
{

}

BossSwiping::BossSwiping(Boss* parent) : NPCState(parent)
{

}

BossSwiping::~BossSwiping() {}