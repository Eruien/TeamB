#include "PlayerReload.h"
#include "LGlobal.h"
#include "LFbxMgr.h"
#include "UIManager.h"

bool PlayerReload::Init()
{
    return true;
}

void PlayerReload::Process()
{
    m_pOwner->IsMove = true;
    m_pOwner->IsEndReload = false;

    if (m_pOwner->IsDeath)
    {
        m_pOwner->SetTransition(Event::FATALDAMAGE);
        return;
    }

    if (m_pOwner->m_TimerEnd)
    {
        m_pOwner->IsMove = true;
        LGlobal::g_BulletCount = 30;
        m_pOwner->SetTransition(Event::ENDRELOAD);
        UIManager::GetInstance().GetUIObject(L"T_Ammo")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(LGlobal::g_BulletCount);
        return;
    }
 
    m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Reload_Rifle_Ironsights.fbx");
}

void PlayerReload::Release()
{

}

PlayerReload::PlayerReload(LPlayer* parent) : PlayerState(parent)
{

}
PlayerReload::~PlayerReload() {}