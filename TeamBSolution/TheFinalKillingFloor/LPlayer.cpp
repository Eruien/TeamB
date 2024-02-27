#include "LPlayer.h"
#include "string"
#include "LWrite.h"
#include "LGlobal.h"
#include "PlayerIdle.h"
#include "PlayerWalk.h"
#include "PlayerRun.h"
#include "PlayerAttack.h"
#include "PlayerReload.h"
#include "PlayerTakeDamage.h"
#include "PlayerDeath.h"
#include "UIManager.h"
#include "LSoundMgr.h"

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
	m_pActionList.insert(std::make_pair(State::CHARACTERTAKEDAMAGE, std::make_unique<PlayerTakeDamage>(this)));
	m_pActionList.insert(std::make_pair(State::CHARACTERDEATH, std::make_unique<PlayerDeath>(this)));

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
		LGlobal::g_EffectSound2->Play(false);
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
		LGlobal::g_EffectSound2->Play(false);
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
		LGlobal::g_EffectSound2->Play(false);
		IsMoveOneDir = true;
		m_AddDirection = m_matControl.Forward() - m_matControl.Right();
		m_AddDirection.Normalize();
		m_matControl._41 -= m_Speed * LGlobal::g_fSPF * m_AddDirection.x;
		m_matControl._43 -= m_Speed * LGlobal::g_fSPF * m_AddDirection.z;
		m_Speed = 50.0f;
	}

	if (LInput::GetInstance().m_KeyStateOld[DIK_S] > KEY_PUSH && LInput::GetInstance().m_KeyStateOld[DIK_D] > KEY_PUSH
		&& !IsMoveOneDir)
	{
		LGlobal::g_EffectSound2->Play(false);
		IsMoveOneDir = true;
		m_AddDirection = m_matControl.Forward() + m_matControl.Right();
		m_AddDirection.Normalize();
		m_matControl._41 -= m_Speed * LGlobal::g_fSPF * m_AddDirection.x;
		m_matControl._43 -= m_Speed * LGlobal::g_fSPF * m_AddDirection.z;
		m_Speed = 50.0f;
	}

	if (LInput::GetInstance().m_KeyStateOld[DIK_W] > KEY_PUSH
		&& !IsMoveOneDir)
	{
		LGlobal::g_EffectSound2->Play(false);
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
		LGlobal::g_EffectSound2->Play(false);
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
		LGlobal::g_EffectSound2->Play(false);
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
		LGlobal::g_EffectSound2->Play(false);
		IsMoveOneDir = true;
		m_AddDirection = m_matControl.Forward();
		m_AddDirection.Normalize();
		m_matControl._41 -= m_Speed * LGlobal::g_fSPF * m_AddDirection.x;
		m_matControl._43 -= m_Speed * LGlobal::g_fSPF * m_AddDirection.z;
		m_Speed = 50.0f;
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
	if (m_HP <= 0)
	{
		IsDeath = true;
	}

	if (IsInvincibility)
	{
		m_StartTakeDamage += LGlobal::g_fSPF;
	}

	if (m_EndTakeDamage < m_StartTakeDamage)
	{
		IsInvincibility = false;
		m_StartTakeDamage = 0.0f;
		IsTakeDamage = false;
	}

	if (IsTakeDamage && !IsInvincibility)
	{
		IsInvincibility = true;
		IsTakeDamage = false;
		IsTakeDammageAni = true;
		m_HP -= 20;
		UIManager::GetInstance().GetUIObject(L"ScreenBlood")->SetIsRender(true);
		UIManager::GetInstance().GetUIObject(L"HPbar")->GetScript<HpBar>(L"HpBar")->UpdateHp();
		if (m_HP <= 61.0f)
		{
			UIManager::GetInstance().GetUIObject(L"face")->GetScript<ChangeTexture>(L"ChangeTexture")->ChangeFromPath(L"../../res/ui/face2.png");
		}
		if (m_HP <= 21.0f)
		{
			UIManager::GetInstance().GetUIObject(L"face")->GetScript<ChangeTexture>(L"ChangeTexture")->ChangeFromPath(L"../../res/ui/face3.png");
		}
	}

	m_StartShoot += LGlobal::g_fSPF;
	IsShoot = false;
	IsReload = false;

	if ((LInput::GetInstance().m_MouseState[0] > KEY_PUSH) && IsEndReload)
	{
		m_Speed = 0.0f;
		IsMove = false;
		IsAttack = true;

		if (m_StartShoot > m_ShotDelay)
		{
			if (LGlobal::g_BulletCount > 0)
			{
				LGlobal::g_EffectSound1->PlayEffect();
			}

			LGlobal::g_BulletCount -= 1;

			if (LGlobal::g_BulletCount <= 0)
			{
				IsReload = true;
				LGlobal::g_BulletCount = 0;
			}

			m_StartShoot = 0.0f;
			IsShoot = true;
			
			UIManager::GetInstance().GetUIObject(L"T_Ammo")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(LGlobal::g_BulletCount);
		}
	}

	if (LInput::GetInstance().m_MouseState[0] == KEY_UP)
	{
		IsMove = true;
		IsAttack = false;
	}

	if (LInput::GetInstance().m_KeyStateOld[DIK_R] == KEY_UP)
	{
		IsReload = true;
	}

	if (LInput::GetInstance().m_KeyStateOld[DIK_F] == KEY_PUSH)
	{
		IsSteamPack = true;
		m_HP -= 10;
		LGlobal::g_SteamPackSound->Play(false);
		UIManager::GetInstance().GetUIObject(L"HPbar")->GetScript<HpBar>(L"HpBar")->UpdateHp();
	}

	if (IsSteamPack)
	{
		m_SteamPackStart += LGlobal::g_fSPF;
		m_AnimationRate = 2.0f;
		float excleSpeed = m_Speed * 2;
		m_Speed = excleSpeed;
	}

	if (m_SteamPackEnd < m_SteamPackStart)
	{
		m_SteamPackStart = 0.0f;
		m_AnimationRate = 1.0f;
		IsSteamPack = false;
	}

	if (m_ZedTimeCount % 10 == 0)
	{
		m_ZedTimeCount += 1;
		IsZedTime = true;
		LGlobal::g_ZedTimeStart->PlayEffect();
	}

	if (IsZedTime)
	{
		m_ZedTimeStart += LGlobal::g_fSPF;
		m_ShotDelay = 0.05f;
		m_AnimationRate = 2.0f;
		float excleSpeed = m_Speed * 2;
		m_Speed = excleSpeed;
	}

	if (m_ZedTimeEnd < m_ZedTimeStart)
	{
		m_ShotDelay = 0.1f;
		m_ZedTimeStart = 0.0f;
		m_AnimationRate = 1.0f;
		IsZedTime = false;
	}

	if (IsMove && IsMovable)
	{
		Move();
	}

	if (LInput::GetInstance().m_KeyStateOld[DIK_LSHIFT] > KEY_PUSH)
	{
		m_Speed = 200.0f;
	}
	else if (LInput::GetInstance().m_KeyStateOld[DIK_LSHIFT] > KEY_PUSH && LInput::GetInstance().m_KeyStateOld[DIK_S] > KEY_PUSH)
	{
		m_Speed = 100.0f;
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

	IsEndReload = true;

	std::wstring wSpeed = L"Speed: ";
	wSpeed += std::to_wstring(int(m_Speed));
	//LWrite::GetInstance().AddText(wSpeed, 0.0f, 150.0f, { 1.0f, 1.0f, 1.0f, 1.0f });

	LSkinningModel::Frame();
	return true;
}