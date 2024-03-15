#include "PlayerAttack.h"

bool PlayerAttack::Init()
{
    return true;
}

void PlayerAttack::Process()
{
    if (m_pOwner->m_Type == PlayerType::GUN)
    {
        m_pOwner->SetTransition(Event::GUNSHOOT);
    }
    else if (m_pOwner->m_Type == PlayerType::SWORD)
    {
        m_pOwner->SetTransition(Event::BLADESLASH);
    }
}

void PlayerAttack::Release()
{

}

PlayerAttack::PlayerAttack(LPlayer* parent) : PlayerState(parent)
{

}
PlayerAttack::~PlayerAttack() {}