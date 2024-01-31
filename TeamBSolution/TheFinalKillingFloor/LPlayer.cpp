#include "LPlayer.h"
#include "string"
#include "LWrite.h"
#include "LGlobal.h"
#include "PlayerIdle.h"
#include "PlayerWalk.h"
#include "PlayerRun.h"
#include "PlayerAttack.h"
#include "PlayerReload.h"

void LPlayer::FSM(FSMType fsmType)
{
	auto iter = LFSMMgr::GetInstance().m_map.find(fsmType);

	if (LFSMMgr::GetInstance().m_map.end() == iter)
	{
		MessageBoxA(NULL, "FSMType Error", "Error Box", MB_OK);
		return;
	}

	m_pFsm = iter->second.get();

	m_pActionList.insert(std::make_pair(State::CHARACTERIDLE, std::make_unique<PlayerIdle>(this)));
	m_pActionList.insert(std::make_pair(State::CHARACTERWALK, std::make_unique<PlayerWalk>(this)));
	m_pActionList.insert(std::make_pair(State::CHARACTERRUN, std::make_unique<PlayerRun>(this)));
	m_pActionList.insert(std::make_pair(State::CHARACTERSHOOT, std::make_unique<PlayerAttack>(this)));
	m_pActionList.insert(std::make_pair(State::CHARACTERRELOAD, std::make_unique<PlayerReload>(this)));

	m_pAction = m_pActionList.find(State::CHARACTERIDLE)->second.get();
	m_CurrentState = State::CHARACTERIDLE;
}

void LPlayer::SetTransition(Event inputEvent)
{
	m_CurrentState = m_pFsm->StateTransition(m_CurrentState, inputEvent);
	m_pAction = m_pActionList.find(m_CurrentState)->second.get();
}

State LPlayer::GetState()
{
	return m_CurrentState;
}

void LPlayer::Process()
{
	m_pAction->Process();
}

void LPlayer::Move()
{
	IsMoveOneDir = false;

	if (LInput::GetInstance().m_KeyStateOld[DIK_W] > KEY_PUSH && LInput::GetInstance().m_KeyStateOld[DIK_A] > KEY_PUSH
		&& !IsMoveOneDir)
	{
		IsMoveOneDir = true;
		m_AddDirection = m_matControl.Forward() + m_matControl.Right();
		m_AddDirection.Normalize();
		m_matControl._41 += m_Speed * LGlobal::g_fSPF * m_AddDirection.x;
		m_matControl._43 += m_Speed * LGlobal::g_fSPF * m_AddDirection.z;
		m_Speed = 100.0f;
	}

	if (LInput::GetInstance().m_KeyStateOld[DIK_W] > KEY_PUSH && LInput::GetInstance().m_KeyStateOld[DIK_D] > KEY_PUSH
		&& !IsMoveOneDir)
	{
		IsMoveOneDir = true;
		m_AddDirection = m_matControl.Forward() - m_matControl.Right();
		m_AddDirection.Normalize();
		m_matControl._41 += m_Speed * LGlobal::g_fSPF * m_AddDirection.x;
		m_matControl._43 += m_Speed * LGlobal::g_fSPF * m_AddDirection.z;
		m_Speed = 100.0f;
	}

	if (LInput::GetInstance().m_KeyStateOld[DIK_S] > KEY_PUSH && LInput::GetInstance().m_KeyStateOld[DIK_A] > KEY_PUSH
		&& !IsMoveOneDir)
	{
		IsMoveOneDir = true;
		m_AddDirection = m_matControl.Forward() - m_matControl.Right();
		m_AddDirection.Normalize();
		m_matControl._41 -= m_Speed * LGlobal::g_fSPF * m_AddDirection.x;
		m_matControl._43 -= m_Speed * LGlobal::g_fSPF * m_AddDirection.z;
		m_Speed = 100.0f;
	}

	if (LInput::GetInstance().m_KeyStateOld[DIK_S] > KEY_PUSH && LInput::GetInstance().m_KeyStateOld[DIK_D] > KEY_PUSH
		&& !IsMoveOneDir)
	{
		IsMoveOneDir = true;
		m_AddDirection = m_matControl.Forward() + m_matControl.Right();
		m_AddDirection.Normalize();
		m_matControl._41 -= m_Speed * LGlobal::g_fSPF * m_AddDirection.x;
		m_matControl._43 -= m_Speed * LGlobal::g_fSPF * m_AddDirection.z;
		m_Speed = 100.0f;
	}

	if (LInput::GetInstance().m_KeyStateOld[DIK_W] > KEY_PUSH
		&& !IsMoveOneDir)
	{
		IsMoveOneDir = true;
		m_AddDirection = m_matControl.Forward();
		m_AddDirection.Normalize();
		m_matControl._41 += m_Speed * LGlobal::g_fSPF * m_AddDirection.x;
		m_matControl._43 += m_Speed * LGlobal::g_fSPF * m_AddDirection.z;
		m_Speed = 100.0f;
	}

	if (LInput::GetInstance().m_KeyStateOld[DIK_A] > KEY_PUSH
		&& !IsMoveOneDir)
	{
		IsMoveOneDir = true;
		m_AddDirection = m_matControl.Right();
		m_AddDirection.Normalize();
		m_matControl._41 += m_Speed * LGlobal::g_fSPF * m_AddDirection.x;
		m_matControl._43 += m_Speed * LGlobal::g_fSPF * m_AddDirection.z;
		m_Speed = 100.0f;
	}

	if (LInput::GetInstance().m_KeyStateOld[DIK_D] > KEY_PUSH
		&& !IsMoveOneDir)
	{
		IsMoveOneDir = true;
		m_AddDirection = m_matControl.Right();
		m_AddDirection.Normalize();
		m_matControl._41 -= m_Speed * LGlobal::g_fSPF * m_AddDirection.x;
		m_matControl._43 -= m_Speed * LGlobal::g_fSPF * m_AddDirection.z;
		m_Speed = 100.0f;
	}

	if (LInput::GetInstance().m_KeyStateOld[DIK_S] > KEY_PUSH
		&& !IsMoveOneDir)
	{
		IsMoveOneDir = true;
		m_AddDirection = m_matControl.Forward();
		m_AddDirection.Normalize();
		m_matControl._41 -= m_Speed * LGlobal::g_fSPF * m_AddDirection.x;
		m_matControl._43 -= m_Speed * LGlobal::g_fSPF * m_AddDirection.z;
		m_Speed = 100.0f;
	}

	if (LInput::GetInstance().m_KeyStateOld[DIK_A] == KEY_UP)
	{
		m_Speed = 0.0f;
	}

	if (LInput::GetInstance().m_KeyStateOld[DIK_D] == KEY_UP)
	{
		m_Speed = 0.0f;
	}

	if (LInput::GetInstance().m_KeyStateOld[DIK_W] == KEY_UP)
	{
		m_Speed = 0.0f;
	}

	if (LInput::GetInstance().m_KeyStateOld[DIK_S] == KEY_UP)
	{
		m_Speed = 0.0f;
	}
}

bool LPlayer::Frame()
{
	m_StartShoot += LGlobal::g_fSPF;
	IsShoot = false;

	if (LInput::GetInstance().m_MouseState[0] > KEY_PUSH)
	{
		m_Speed = 0.0f;
		IsMove = false;
		IsAttack = true;
		
		if (m_StartShoot > m_ShotDelay)
		{
			m_StartShoot = 0.0f;
			IsShoot = true;
			m_BulletCount -= 1;

			if (m_BulletCount <= 0)
			{
				IsReload = true;
			}
		}
	}

	if (LInput::GetInstance().m_MouseState[0] == KEY_UP)
	{
		IsMove = true;
		IsAttack = false;
		IsReload = false;
	}

	if (IsMove && IsMovable)
	{
		Move();
	}

	if (LInput::GetInstance().m_KeyStateOld[DIK_LSHIFT] > KEY_PUSH)
	{
		m_Speed = 200.0f;
	}

	if (LInput::GetInstance().m_KeyStateOld[DIK_LSHIFT] == KEY_UP)
	{
		m_Speed = 0.0f;
	}

	if (m_Speed > 150.0f)
	{
		IsRun = true;
		IsWalk = true;
	}
	else if (m_Speed > 0.0f)
	{
		IsRun = false;
		IsWalk = true;
	}
	else if (m_Speed <= 0.0f)
	{
		IsRun = false;
		IsWalk = false;
	}

	std::wstring wSpeed = L"Speed: ";
	wSpeed += std::to_wstring(int(m_Speed));
	LWrite::GetInstance().AddText(wSpeed, 0.0f, 150.0f, { 1.0f, 1.0f, 1.0f, 1.0f });

	LSkinningModel::Frame();
	return true;
}