#include "PlayerAttack.h"
#include "LGlobal.h"
#include "LFbxMgr.h"

bool PlayerAttack::Init()
{
    return true;
}

void PlayerAttack::Process()
{
    if (!m_pOwner->IsAttack)
    {
        m_pOwner->SetTransition(Event::ENDATTACK);
        return;
    }

    m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Fire_Rifle_7.fbx");
}

void PlayerAttack::Release()
{

}

PlayerAttack::PlayerAttack(LPlayer* parent) : PlayerState(parent)
{

}
PlayerAttack::~PlayerAttack() {}