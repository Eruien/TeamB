#include "PlayerAttack.h"
#include "LGlobal.h"
#include "LFbxMgr.h"
#include "LInput.h"

bool PlayerAttack::Montage(int startFrame, int endFrame)
{
    if (startFrame <= m_pOwner->m_fCurrentAnimTime && endFrame >= m_pOwner->m_fCurrentAnimTime)
    {
        return true;
    }
    return false;
}

bool PlayerAttack::Init()
{
    return true;
}

void PlayerAttack::Process()
{
    m_pOwner->m_pActionModel->m_iStartFrame;
    m_pOwner->m_pActionModel->m_iEndFrame;
    if (m_CurrentCombo == ComboType::OUTWARD)
    {
        if (Montage(20, 38))
        {
            m_pOwner->IsSlash = true;
        }
        else
        {
            m_pOwner->IsSlash = false;
        }
    }
    else if (m_CurrentCombo == ComboType::INWARD)
    {
        if (Montage(34, 52))
        {
            m_pOwner->IsSlash = true;
        }
        else
        {
            m_pOwner->IsSlash = false;
        }
    }
    
    if (LInput::GetInstance().m_MouseState[0] == KEY_PUSH && !IsFirstClick)
    {
        IsClick = true;
        IsFirstClick = true;
    }

    if (IsClick && m_pOwner->m_TimerEnd)
    {
        m_pOwner->m_TimerEnd = false;
        IsClick = false;
        m_CurrentCombo = ComboType::OUTWARD;
    }

   
    
    if (m_pOwner->IsDeath)
    {
        m_pOwner->IsResetBladeAttack = true;
        IsClick = false;
        IsFirstClick = false;
        m_CurrentCombo = ComboType::INWARD;
        m_pOwner->SetTransition(Event::FATALDAMAGE);
        return;
    }

    if (m_pOwner->IsTakeDammageAni)
    {
        m_pOwner->IsResetBladeAttack = true;
        IsClick = false;
        IsFirstClick = false;
        m_CurrentCombo = ComboType::INWARD;
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
            IsClick = false;
            IsFirstClick = false;
            m_CurrentCombo = ComboType::INWARD;
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
        if (m_CurrentCombo == ComboType::INWARD)
        {
            m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"OneHand_Inward.fbx");
        }
        else if(m_CurrentCombo == ComboType::OUTWARD)
        {
            m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"OneHand_Outward.fbx");
        }
    }
}

void PlayerAttack::Release()
{

}

PlayerAttack::PlayerAttack(LPlayer* parent) : PlayerState(parent)
{

}
PlayerAttack::~PlayerAttack() {}