#include "TankRush.h"
#include "LGlobal.h"
#include "LFbxMgr.h"
#include "UIManager.h"

bool TankRush::Init()
{
    return true;
}

void TankRush::Process()
{
    if (m_pOwner->IsTakeDamage)
    {
        m_pOwner->SetTransition(Event::TAKEDAMAGE);
        return;
    }

    if (!m_pOwner->IsRush)
    {
        m_pOwner->SetTransition(Event::ENDATTACK);
        return;
    }

    if (m_pOwner->IsMovable)
    {
        m_pOwner->RushMove();
    }

    m_pOwner->m_pActionModel = LFbxMgr::GetInstance().GetPtr(L"Tank_Run.fbx");
}

void TankRush::Release()
{

}

TankRush::TankRush(Tank* parent) : TankState(parent)
{

}
TankRush::~TankRush() {}