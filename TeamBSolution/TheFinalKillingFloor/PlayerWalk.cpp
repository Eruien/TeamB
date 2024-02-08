#include "PlayerWalk.h"
#include "LGlobal.h"
#include "LFbxMgr.h"

bool PlayerWalk::Init()
{
    return true;
}

void PlayerWalk::Process()
{
    if (m_pOwner->IsDeath)
    {
        m_pOwner->SetTransition(Event::FATALDAMAGE);
        return;
    }

    if (m_pOwner->IsTakeDammageAni)
    {
        m_pOwner->SetTransition(Event::TAKEDAMAGE);
        return;
    }

    if (m_pOwner->IsReload)
    {
        m_pOwner->SetTransition(Event::STARTRELOAD);
        return;
    }

    if (m_pOwner->IsAttack)
    {
        m_pOwner->SetTransition(Event::CLICKATTACKBUTTON);
        return;
    }

    if (m_pOwner->IsRun)
    {
        m_pOwner->SetTransition(Event::RUNSPEED);
        return;
    }

    if (!m_pOwner->IsWalk)
    {
        m_pOwner->SetTransition(Event::IDLESPEED);
        return;
    }

    m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Walk_Fwd_Rifle_Ironsights.fbx");
}

void PlayerWalk::Release()
{

}

PlayerWalk::PlayerWalk(LPlayer* parent) : PlayerState(parent)
{

}
PlayerWalk::~PlayerWalk() {}