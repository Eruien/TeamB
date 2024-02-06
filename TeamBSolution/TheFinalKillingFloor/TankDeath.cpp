#include "TankDeath.h"
#include "LGlobal.h"
#include "LFbxMgr.h"
#include "UIManager.h"

bool TankDeath::Init()
{
    return true;
}

void TankDeath::Process()
{
    m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Zombie_Death.fbx");
    m_pOwner->IsDead = true;
}

void TankDeath::Release()
{

}

TankDeath::TankDeath(Tank* parent) : TankState(parent)
{

}
TankDeath::~TankDeath() {}