#include "PlayerDeath.h"
#include "LGlobal.h"
#include "LFbxMgr.h"

bool PlayerDeath::Init()
{
    return true;
}

void PlayerDeath::Process()
{
    if (m_pOwner->m_TimerEnd)
    {
        m_pOwner->IsDeath = false;
        LGlobal::g_HP = 100.0f;
        m_pOwner->SetTransition(Event::CHARACTERREVIVE);
        return;
    }

    m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Player_Death.fbx");
}

void PlayerDeath::Release()
{

}

PlayerDeath::PlayerDeath(LPlayer* parent) : PlayerState(parent)
{

}
PlayerDeath::~PlayerDeath() {}