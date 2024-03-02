#include "TankRush.h"
#include "LGlobal.h"
#include "LFbxMgr.h"
#include "UIManager.h"
#include "KObject.h"

bool TankRush::Init()
{
    return true;
}

void TankRush::Process()
{
    if (m_pOwner->IsTakeDamage && LGlobal::g_BulletCount > 0)
    {
        m_pOwner->m_HP -= 3.0f;
        UpdateHPbar();
        m_pOwner->IsTakeDamage = false;
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

void TankRush::UpdateHPbar()
{
    float hp = LGlobal::g_PlayerModel->m_HP;

    m_pOwner->m_enemyHp->m_VertexList[1].p.x = 0.5f - (1 - m_pOwner->m_HP / 100);
    m_pOwner->m_enemyHp->m_VertexList[4].p.x = 0.5f - (1 - m_pOwner->m_HP / 100);
    m_pOwner->m_enemyHp->m_VertexList[5].p.x = 0.5f - (1 - m_pOwner->m_HP / 100);

    D3D11_BUFFER_DESC bufferDesc = { 0, };
    bufferDesc.ByteWidth = sizeof(SimpleVertex) * m_pOwner->m_enemyHp->m_VertexList.size();
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA initialData = { 0, };
    initialData.pSysMem = &m_pOwner->m_enemyHp->m_VertexList.at(0);

    HRESULT hr = m_pOwner->m_enemyHp->m_pDevice->CreateBuffer(
        &bufferDesc,
        &initialData,
        &m_pOwner->m_enemyHp->m_pVertexBuffer);

    if (FAILED(hr))
    {
        MessageBoxA(NULL, "Create Buffer Error", "Error Box", MB_OK);
        return;
    }
}

TankRush::TankRush(Tank* parent) : TankState(parent)
{

}
TankRush::~TankRush() {}