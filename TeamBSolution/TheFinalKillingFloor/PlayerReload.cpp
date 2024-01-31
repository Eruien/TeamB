#include "PlayerReload.h"
#include "LGlobal.h"
#include "LFbxMgr.h"

bool PlayerReload::Init()
{
    return true;
}

void PlayerReload::Process()
{
    if (m_pOwner->m_TimerEnd)
    {
        m_pOwner->m_BulletCount = 30;
        m_pOwner->SetTransition(Event::ENDRELOAD);
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