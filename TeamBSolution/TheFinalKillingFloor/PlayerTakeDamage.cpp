#include "PlayerTakeDamage.h"
#include "LGlobal.h"
#include "LFbxMgr.h"

bool PlayerTakeDamage::Init()
{
    return true;
}

void PlayerTakeDamage::Process()
{
<<<<<<< Updated upstream
    m_pOwner->IsMove = true;
    m_pOwner->IsTakeDammageAni = false;

=======
    
>>>>>>> Stashed changes
    if (m_pOwner->IsDeath)
    {
        m_pOwner->SetTransition(Event::FATALDAMAGE);
        return;
    }

    if (m_pOwner->m_TimerEnd)
    {
        m_pOwner->SetTransition(Event::RECOVERYDAMAGE);
        return;
    }

    m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Player_TakeDamage.fbx");
}

void PlayerTakeDamage::Release()
{

}

PlayerTakeDamage::PlayerTakeDamage(LPlayer* parent) : PlayerState(parent)
{

}
PlayerTakeDamage::~PlayerTakeDamage() {}