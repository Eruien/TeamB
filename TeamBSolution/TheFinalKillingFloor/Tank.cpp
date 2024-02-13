#include "Tank.h"
#include "string"
#include "LWrite.h"
#include "LGlobal.h"
#include "TankTrace.h"
#include "TankCombo.h"
#include "TankRush.h"
#include "TankTakeDamage.h"
#include "TankDeath.h"
#include "KObject.h"

void Tank::FSM(FSMType fsmType)
{
	auto iter = LFSMMgr::GetInstance().m_map.find(fsmType);

	if (LFSMMgr::GetInstance().m_map.end() == iter)
	{
		MessageBoxA(NULL, "FSMType Error", "Error Box", MB_OK);
		return;
	}

	m_pFsm = iter->second.get();

	m_pActionList.insert(std::make_pair(State::TANKTRACE, std::make_unique<TankTrace>(this)));
	m_pActionList.insert(std::make_pair(State::TANKCOMBO, std::make_unique<TankCombo>(this)));
	m_pActionList.insert(std::make_pair(State::TANKRUSH, std::make_unique<TankRush>(this)));
	m_pActionList.insert(std::make_pair(State::TANKTAKEDAMAGE, std::make_unique<TankTakeDamage>(this)));
	m_pActionList.insert(std::make_pair(State::TANKDEATH, std::make_unique<TankDeath>(this)));

	m_pAction = m_pActionList.find(State::TANKTRACE)->second.get();
	m_CurrentState = State::TANKTRACE;
}

void Tank::SetTransition(Event inputEvent)
{
	m_CurrentState = m_pFsm->StateTransition(m_CurrentState, inputEvent);
	m_pAction = m_pActionList.find(m_CurrentState)->second.get();
}

State Tank::GetState()
{
	return m_CurrentState;
}

void Tank::Process()
{
	m_pAction->Process();
}

void Tank::Move(TVector3 target)
{
	m_Dir = target - TVector3(m_matControl._41, m_matControl._42, m_matControl._43);
	m_Dir.Normalize();
	TVector3 forward = m_matControl.Forward();

	float dirX = m_Dir.x;
	float dirZ = m_Dir.z;

	DirectX::XMVECTOR gRotation;
	DirectX::XMMATRIX matRotation;

	float yawRadians = atan2(dirZ, dirX);
	gRotation = DirectX::XMQuaternionRotationRollPitchYaw(0, -yawRadians - 1.5708, 0);
	DirectX::XMVECTOR xmPos = DirectX::XMVectorSet(m_matControl._41, m_matControl._42, m_matControl._43, 1.0f);
	matRotation = DirectX::XMMatrixAffineTransformation(DirectX::g_XMOne, DirectX::g_XMZero, gRotation, xmPos);

	TMatrix zombieScale;
	D3DXMatrixScaling(&zombieScale, 0.2f, 0.2f, 0.2f);
	TMatrix zombiePos = zombieScale * matRotation;

	m_matControl = zombiePos;
	m_matControl._41 += m_Speed * LGlobal::g_fSPF * m_Dir.x;
	m_matControl._43 += m_Speed * LGlobal::g_fSPF * m_Dir.z;
}

void Tank::RushMove()
{
	TVector3 forward = m_matControl.Forward();
	forward.Normalize();
	m_matControl._41 += m_RushSpeed * LGlobal::g_fSPF * forward.x;
	m_matControl._43 += m_RushSpeed * LGlobal::g_fSPF * forward.z;
}

void Tank::ComboMove()
{
	TVector3 forward = m_matControl.Forward();
	forward.Normalize();
	m_matControl._41 += m_ComboSpeed * LGlobal::g_fSPF * forward.x;
	m_matControl._43 += m_ComboSpeed * LGlobal::g_fSPF * forward.z;
}

int Tank::GetRandomNumber()
{
	return m_Distribution(m_Generator);
}

bool Tank::Frame()
{
	m_ZombieSound->Play();
	// ������

	m_enemyHp->SetPos({ m_matControl._41,m_matControl._42 + 55, m_matControl._43 });

	TMatrix matRotation, matTrans, matScale, worldMat;
	D3DXMatrixInverse(&matRotation, nullptr, &LGlobal::g_pMainCamera->m_matView);
	matRotation._41 = 0.0f;
	matRotation._42 = 0.0f;
	matRotation._43 = 0.0f;
	matRotation._44 = 1.0f;

	D3DXMatrixTranslation(&matTrans, m_enemyHp->m_vPosition.x,
		m_enemyHp->m_vPosition.y,
		m_enemyHp->m_vPosition.z
	);

	D3DXMatrixScaling(&matScale, m_enemyHp->m_vScale.x,
		m_enemyHp->m_vScale.y,
		m_enemyHp->m_vScale.z
	);
	worldMat = matScale * matRotation * matTrans;
	m_enemyHp->SetMatrix(&worldMat, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
	m_enemyHp->Frame();
	// ������ ��

	if (m_HP <= 0)
	{
		IsDead = true;
	}

	LSkinningModel::Frame();

	m_PlayerPos = { m_Player->m_matControl._41, m_Player->m_matControl._42, m_Player->m_matControl._43 };
	m_NPCPos = { m_matControl._41, m_matControl._42, m_matControl._43 };

	if (!IsUseRush)
	{
		m_RushCoolTimeStart += LGlobal::g_fSPF;
	}

	if (m_RushCoolTimeStart > m_RushCoolTimeEnd)
	{
		m_RushCoolTimeStart = 0.0f;
		IsUseRush = true;
	}

	if (IsRush)
	{
		m_RushStart += LGlobal::g_fSPF;
	}

	if (m_RushStart > m_RushEnd)
	{
		m_RushStart = 0.0f;
		IsRush = false;
		IsRushDir = true;
		IsUseRush = false;
	}

	if (((m_NPCPos.x - m_RushRange) < m_PlayerPos.x) && (m_PlayerPos.x < (m_NPCPos.x + m_RushRange))
		&& ((m_NPCPos.z - m_RushRange) < m_PlayerPos.z) && (m_PlayerPos.z < (m_NPCPos.z + m_RushRange)) && IsRushDir && IsUseRush)
	{
		m_RushPos = m_PlayerPos;
		IsRush = true;
		IsRushDir = false;
	}

	if (((m_NPCPos.x - m_ComboRange) < m_PlayerPos.x) && (m_PlayerPos.x < (m_NPCPos.x + m_ComboRange))
		&& ((m_NPCPos.z - m_ComboRange) < m_PlayerPos.z) && (m_PlayerPos.z < (m_NPCPos.z + m_ComboRange)))
	{
		IsComboRange = true;
	}
	else
	{
		IsComboRange = false;
	}

	return true;
}

bool Tank::Render()
{
	LSkinningModel::Render();
	m_enemyHp->Render();
	return true;
}

bool Tank::RenderMark()
{
	m_minimapMarker->Render();
	return false;
}

Tank::Tank(LPlayer* player) : m_Distribution(-3000, 3000)
{
	m_Player = player;
	m_ZombieSound = LSoundMgr::GetInstance().Load(L"../../res/sound/zombieSound.mp3");
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	m_Generator.seed(seed);

	m_RandomPos = { float(GetRandomNumber()), 0.0f, float(GetRandomNumber()) };

	m_enemyHp = make_shared<KObject>();
	m_enemyHp->Init();
	m_enemyHp->Create(L"../../res/hlsl/CustomizeMap.hlsl", L"../../res/ui/hp_bar.png");
	m_enemyHp->SetPos({ 0, 0, 0 });
	m_enemyHp->SetScale({ 18,3,1 });

	m_minimapMarker = make_shared<KObject>();
	m_minimapMarker->Init();
	m_minimapMarker->SetPos({ 0, 0, -1 });
	m_minimapMarker->SetScale({ 20,20, 1 });
	m_minimapMarker->Create(L"../../res/hlsl/CustomizeMap.hlsl", L"../../res/ui/firemode_dot.png");
}