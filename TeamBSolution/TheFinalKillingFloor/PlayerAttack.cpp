#include "PlayerAttack.h"
#include "LGlobal.h"
#include "LFbxMgr.h"

bool PlayerAttack::Init()
{
    return true;
}

void PlayerAttack::Process()
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

    if (!m_pOwner->IsAttack)
    {
        m_pOwner->SetTransition(Event::ENDATTACK);
        return;
    }

    if (m_pOwner->m_CurrentGun == WeaponState::PISTOL)
    {
        m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Pistol_Shoot.fbx");
    }
    else if (m_pOwner->m_CurrentGun == WeaponState::ASSAULTRIFLE)
    {
        m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Fire_Rifle_Ironsights.fbx");
    }
    
}

void PlayerAttack::Release()
{

}

PlayerAttack::PlayerAttack(LPlayer* parent) : PlayerState(parent)
{

}
PlayerAttack::~PlayerAttack() {}