#include "PlayerBladeSlash.h"
#include "LGlobal.h"
#include "LFbxMgr.h"
#include "LInput.h"

bool PlayerBladeSlash::Montage(int startFrame, int endFrame)
{
    if (startFrame <= m_pOwner->m_fCurrentAnimTime && endFrame >= m_pOwner->m_fCurrentAnimTime)
    {
        return true;
    }
    return false;
}

bool PlayerBladeSlash::Init()
{
    return true;
}

void PlayerBladeSlash::Process()
{
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

    if (LInput::GetInstance().m_MouseState[0] >= KEY_PUSH && !IsFirstClick)
    {
        IsFirstClick = true;
        IsClick = true;
    }

    if (IsClick && m_pOwner->m_TimerEnd)
    {
        m_pOwner->IsResetBladeAttack = true;
        m_pOwner->m_TimerEnd = false;
        IsClick = false;
        m_CurrentCombo = ComboType::OUTWARD;
    }

    if (m_pOwner->IsDeath)
    {
        m_pOwner->IsResetBladeAttack = true;
        IsFirstClick = false;
        IsClick = false;
        m_CurrentCombo = ComboType::INWARD;
        m_pOwner->SetTransition(Event::FATALDAMAGE);
        return;
    }

    if (m_pOwner->m_TimerEnd)
    {
        m_pOwner->IsResetBladeAttack = true;
        IsFirstClick = false;
        IsClick = false;
        m_CurrentCombo = ComboType::INWARD;
        m_pOwner->IsTakeDammageAni = false;
        m_pOwner->SetTransition(Event::ENDATTACK);
        return;
    }
  
    if (m_CurrentCombo == ComboType::INWARD)
    {
        m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"OneHand_Inward.fbx");
    }
    else if (m_CurrentCombo == ComboType::OUTWARD)
    {
        m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"OneHand_Outward.fbx");
    }
}

void PlayerBladeSlash::Release()
{

}

PlayerBladeSlash::PlayerBladeSlash(LPlayer* parent) : PlayerState(parent)
{

}
PlayerBladeSlash::~PlayerBladeSlash() {}