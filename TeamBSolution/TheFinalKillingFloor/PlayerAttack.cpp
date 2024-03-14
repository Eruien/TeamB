#include "PlayerAttack.h"
#include "LGlobal.h"
#include "LFbxMgr.h"

bool PlayerAttack::Init()
{
    return true;
}

void PlayerAttack::Process()
{
    m_pOwner->IsSlash = true;

    if (m_pOwner->IsDeath)
    {
        m_pOwner->IsResetBladeAttack = true;
        m_pOwner->IsSlash = false;
        m_pOwner->SetTransition(Event::FATALDAMAGE);
        return;
    }

    if (m_pOwner->IsTakeDammageAni)
    {
        m_pOwner->IsResetBladeAttack = true;
        m_pOwner->IsSlash = false;
        m_pOwner->SetTransition(Event::TAKEDAMAGE);
        return;
    }

    if (m_pOwner->m_Type == PlayerType::GUN)
    {
        if (m_pOwner->IsReload)
        {
            m_pOwner->SetTransition(Event::STARTRELOAD);
            return;
        }
    }
  
    if (m_pOwner->m_Type == PlayerType::GUN)
    {
        if (!m_pOwner->IsAttack)
        {
            m_pOwner->SetTransition(Event::ENDATTACK);
            return;
        }
    }
    else if (m_pOwner->m_Type == PlayerType::SWORD)
    {
        if (m_pOwner->m_TimerEnd)
        {
            m_pOwner->IsResetBladeAttack = true;
            m_pOwner->IsSlash = false;
            m_pOwner->SetTransition(Event::ENDATTACK);
            return;
        }
    }
  
    if (m_pOwner->m_CurrentGun == WeaponState::PISTOL)
    {
        //m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Pistol_Shoot.fbx");
        m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Pistol_Idle.fbx");
    }
    else if (m_pOwner->m_CurrentGun == WeaponState::ASSAULTRIFLE)
    {
        m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Fire_Rifle_Ironsights.fbx");
    }
    else if (m_pOwner->m_CurrentGun == WeaponState::SHOTGUN)
    {
        m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Shotgun_Fire.fbx");
    }
    else if (m_pOwner->m_CurrentGun == WeaponState::ONEHANDSWORD)
    {
        m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"OneHand_Outward.fbx");
    }
    
}

void PlayerAttack::Release()
{

}

PlayerAttack::PlayerAttack(LPlayer* parent) : PlayerState(parent)
{

}
PlayerAttack::~PlayerAttack() {}