#include "LPlayer.h"
#include "string"
#include "LWrite.h"
#include "LGlobal.h"
#include "LFbxMgr.h"
#include "LExportIO.h"
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
	if (!IsKillable) m_HP = 100.f;
	if (LInput::GetInstance().m_KeyStateOld[DIK_Q] == KEY_PUSH)
		IsKillable = !IsKillable;
	m_pAction->Process();
}

void LPlayer::Move()
{
	if (IsOnAir)
	{
		m_matForAnim = m_matControl;
		if (accumulatedTime > 0) accumulatedTime = max(0, accumulatedTime - LGlobal::g_fSPF * 150);
		else if (accumulatedTime < 0) accumulatedTime = min(0, accumulatedTime + LGlobal::g_fSPF * 150);
		if (LInput::GetInstance().m_KeyStateOld[DIK_A] > KEY_PUSH)
		{
			if (LInput::GetInstance().m_MouseState[0] == KEY_FREE && accumulatedTime > -0.1f)
			{
				accumulatedTime = min(20, accumulatedTime + LGlobal::g_fSPF * 300);
			}
		}
		else if (LInput::GetInstance().m_KeyStateOld[DIK_D] > KEY_PUSH)
		{
			if (LInput::GetInstance().m_MouseState[0] == KEY_FREE && accumulatedTime < 0.1f)
			{
				accumulatedTime = max(-20, accumulatedTime - LGlobal::g_fSPF * 300);
			}
		}
		if (abs(accumulatedTime) > 0.001f)
		{
			float angle = -accumulatedTime;

			float rotateAngle = XMConvertToRadians(angle); //
			TMatrix matRotate;
			D3DXMatrixRotationY(&matRotate, rotateAngle); //
			m_matForAnim *= matRotate; // m_matForAnim에 회전 변환 적용
			m_matForAnim._41 = m_matControl._41;
			m_matForAnim._42 = m_matControl._42;
			m_matForAnim._43 = m_matControl._43;
		}
		return;
	}
	IsMoveOneDir = false;
	m_PrevPosition = { m_matControl._41, m_matControl._42, m_matControl._43 };
	m_matForAnim = m_matControl;
	if (accumulatedTime > 0) accumulatedTime = max(0, accumulatedTime - LGlobal::g_fSPF * 150);
	else if (accumulatedTime < 0) accumulatedTime = min(0, accumulatedTime + LGlobal::g_fSPF * 150);


	if (LInput::GetInstance().m_KeyStateOld[DIK_W] > KEY_PUSH && LInput::GetInstance().m_KeyStateOld[DIK_A] > KEY_PUSH
		&& !IsMoveOneDir)
	{
		LGlobal::g_EffectSound2->Play(true);
		IsMoveOneDir = true;
		m_AddDirection = m_matControl.Forward() + m_matControl.Right();
		m_AddDirection.Normalize();
		m_matControl._41 += m_Speed * LGlobal::g_fSPF * m_AddDirection.x;
		m_matControl._43 += m_Speed * LGlobal::g_fSPF * m_AddDirection.z;
		m_Speed = FRONTMOVESPEED;

		
		
		if (LInput::GetInstance().m_MouseState[0] == KEY_FREE && accumulatedTime > - 0.1f)
		{
			accumulatedTime  = min(20, accumulatedTime + LGlobal::g_fSPF * 300);
		}
	}

	if (LInput::GetInstance().m_KeyStateOld[DIK_W] > KEY_PUSH && LInput::GetInstance().m_KeyStateOld[DIK_D] > KEY_PUSH
		&& !IsMoveOneDir)
	{
		LGlobal::g_EffectSound2->Play(true);
		IsMoveOneDir = true;
		m_AddDirection = m_matControl.Forward() - m_matControl.Right();
		m_AddDirection.Normalize();
		m_matControl._41 += m_Speed * LGlobal::g_fSPF * m_AddDirection.x;
		m_matControl._43 += m_Speed * LGlobal::g_fSPF * m_AddDirection.z;
		m_Speed = 100.0f;

		if (LInput::GetInstance().m_MouseState[0] == KEY_FREE && accumulatedTime < 0.1f)
		{
			accumulatedTime = max(-20, accumulatedTime - LGlobal::g_fSPF * 300);
		}
	}
	if (abs(accumulatedTime) > 0.001f)
	{
		float angle = -accumulatedTime;

		float rotateAngle = XMConvertToRadians(angle); //
		TMatrix matRotate;
		D3DXMatrixRotationY(&matRotate, rotateAngle); //
		m_matForAnim *= matRotate; // m_matForAnim에 회전 변환 적용
		m_matForAnim._41 = m_matControl._41;
		m_matForAnim._42 = m_matControl._42;
		m_matForAnim._43 = m_matControl._43;
	}


	if (LInput::GetInstance().m_KeyStateOld[DIK_S] > KEY_PUSH && LInput::GetInstance().m_KeyStateOld[DIK_A] > KEY_PUSH
		&& !IsMoveOneDir)
	{
		LGlobal::g_EffectSound2->Play(true);
		IsMoveOneDir = true;
		m_AddDirection = m_matControl.Forward() - m_matControl.Right();
		m_AddDirection.Normalize();
		m_matControl._41 -= m_Speed * LGlobal::g_fSPF * m_AddDirection.x;
		m_matControl._43 -= m_Speed * LGlobal::g_fSPF * m_AddDirection.z;
		m_Speed = BACKMOVESPEED;
	}

	if (LInput::GetInstance().m_KeyStateOld[DIK_S] > KEY_PUSH && LInput::GetInstance().m_KeyStateOld[DIK_D] > KEY_PUSH
		&& !IsMoveOneDir)
	{
		LGlobal::g_EffectSound2->Play(true);
		IsMoveOneDir = true;
		m_AddDirection = m_matControl.Forward() + m_matControl.Right();
		m_AddDirection.Normalize();
		m_matControl._41 -= m_Speed * LGlobal::g_fSPF * m_AddDirection.x;
		m_matControl._43 -= m_Speed * LGlobal::g_fSPF * m_AddDirection.z;
		m_Speed = BACKMOVESPEED;
	}

	if (LInput::GetInstance().m_KeyStateOld[DIK_W] > KEY_PUSH
		&& !IsMoveOneDir)
	{
		LGlobal::g_EffectSound2->Play(true);
		IsMoveOneDir = true;
		m_AddDirection = m_matControl.Forward();
		m_AddDirection.Normalize();
		m_matControl._41 += m_Speed * LGlobal::g_fSPF * m_AddDirection.x;
		m_matControl._43 += m_Speed * LGlobal::g_fSPF * m_AddDirection.z;
		m_Speed = FRONTMOVESPEED;
	}

	if (LInput::GetInstance().m_KeyStateOld[DIK_A] > KEY_PUSH
		&& !IsMoveOneDir)
	{
		LGlobal::g_EffectSound2->Play(true);
		IsMoveOneDir = true;
		m_AddDirection = m_matControl.Right();
		m_AddDirection.Normalize();
		m_matControl._41 += m_Speed * LGlobal::g_fSPF * m_AddDirection.x;
		m_matControl._43 += m_Speed * LGlobal::g_fSPF * m_AddDirection.z;
		m_Speed = FRONTMOVESPEED;
	}

	if (LInput::GetInstance().m_KeyStateOld[DIK_D] > KEY_PUSH
		&& !IsMoveOneDir)
	{
		LGlobal::g_EffectSound2->Play(true);
		IsMoveOneDir = true;
		m_AddDirection = m_matControl.Right();
		m_AddDirection.Normalize();
		m_matControl._41 -= m_Speed * LGlobal::g_fSPF * m_AddDirection.x;
		m_matControl._43 -= m_Speed * LGlobal::g_fSPF * m_AddDirection.z;
		m_Speed = FRONTMOVESPEED;
	}

	if (LInput::GetInstance().m_KeyStateOld[DIK_S] > KEY_PUSH
		&& !IsMoveOneDir)
	{
		LGlobal::g_EffectSound2->Play(true);
		IsMoveOneDir = true;
		m_AddDirection = m_matControl.Forward();
		m_AddDirection.Normalize();
		m_matControl._41 -= m_Speed * LGlobal::g_fSPF * m_AddDirection.x;
		m_matControl._43 -= m_Speed * LGlobal::g_fSPF * m_AddDirection.z;
		m_Speed = BACKMOVESPEED;
	}

	if (LInput::GetInstance().m_KeyStateOld[DIK_A] == KEY_UP)
	{
		LGlobal::g_EffectSound2->Stop();
		m_Speed = 0.0f;
	}

	if (LInput::GetInstance().m_KeyStateOld[DIK_D] == KEY_UP)
	{
		LGlobal::g_EffectSound2->Stop();
		m_Speed = 0.0f;
	}

	if (LInput::GetInstance().m_KeyStateOld[DIK_W] == KEY_UP)
	{
		LGlobal::g_EffectSound2->Stop();
		m_Speed = 0.0f;
	}

	if (LInput::GetInstance().m_KeyStateOld[DIK_S] == KEY_UP)
	{
		LGlobal::g_EffectSound2->Stop();
		m_Speed = 0.0f;
	}

	if (LInput::GetInstance().m_KeyStateOld[DIK_SPACE] == KEY_PUSH
		&& IsOnAir == false)
	{
		m_Velocity = { 0.0f, 100.0f, 0.0f };
		if (LInput::GetInstance().m_KeyStateOld[DIK_W] > KEY_PUSH)
		{
			if (LInput::GetInstance().m_KeyStateOld[DIK_LSHIFT] > KEY_PUSH)
			{
				if (IsSteamPack)
				{
					if (LInput::GetInstance().m_KeyStateOld[DIK_A] > KEY_PUSH)
					{
						m_Velocity.x = (m_matControl.Forward().x + m_matControl.Right().x) * 1000.0f;
						m_Velocity.z = (m_matControl.Forward().z + m_matControl.Right().z) * 1000.0f;
					}
					else if (LInput::GetInstance().m_KeyStateOld[DIK_D] > KEY_PUSH)
					{
						m_Velocity.x = (m_matControl.Forward().x - m_matControl.Right().x) * 1000.0f;
						m_Velocity.z = (m_matControl.Forward().z - m_matControl.Right().z) * 1000.0f;
					}
					else
					{
						m_Velocity.x = m_matControl.Forward().x * 2000.0f;
						m_Velocity.z = m_matControl.Forward().z * 2000.0f;
					}
				}
				else
				{
					if (LInput::GetInstance().m_KeyStateOld[DIK_A] > KEY_PUSH)
					{
						m_Velocity.x = (m_matControl.Forward().x + m_matControl.Right().x) * 1000.0f;
						m_Velocity.z = (m_matControl.Forward().z + m_matControl.Right().z) * 1000.0f;
					}
					else if (LInput::GetInstance().m_KeyStateOld[DIK_D] > KEY_PUSH)
					{
						m_Velocity.x = (m_matControl.Forward().x - m_matControl.Right().x) * 1000.0f;
						m_Velocity.z = (m_matControl.Forward().z - m_matControl.Right().z) * 1000.0f;
					}
					else
					{
						m_Velocity.x = m_matControl.Forward().x * 2000.0f;
						m_Velocity.z = m_matControl.Forward().z * 2000.0f;
					}
				}
			}
			else
			{
				if (IsSteamPack)
				{
					if (LInput::GetInstance().m_KeyStateOld[DIK_A] > KEY_PUSH)
					{
						m_Velocity.x = (m_matControl.Forward().x + m_matControl.Right().x) * 1000.0f;
						m_Velocity.z = (m_matControl.Forward().z + m_matControl.Right().z) * 1000.0f;
					}
					else if (LInput::GetInstance().m_KeyStateOld[DIK_D] > KEY_PUSH)
					{
						m_Velocity.x = (m_matControl.Forward().x - m_matControl.Right().x) * 1000.0f;
						m_Velocity.z = (m_matControl.Forward().z - m_matControl.Right().z) * 1000.0f;
					}
					else
					{
						m_Velocity.x = m_matControl.Forward().x * 1500.0f;
						m_Velocity.z = m_matControl.Forward().z * 1500.0f;
					}
				}
				else
				{
					if (LInput::GetInstance().m_KeyStateOld[DIK_A] > KEY_PUSH)
					{
						m_Velocity.x = (m_matControl.Forward().x + m_matControl.Right().x) * 1000.0f;
						m_Velocity.z = (m_matControl.Forward().z + m_matControl.Right().z) * 1000.0f;
					}
					else if (LInput::GetInstance().m_KeyStateOld[DIK_D] > KEY_PUSH)
					{
						m_Velocity.x = (m_matControl.Forward().x - m_matControl.Right().x) * 1000.0f;
						m_Velocity.z = (m_matControl.Forward().z - m_matControl.Right().z) * 1000.0f;
					}
					else
					{
						m_Velocity.x = m_matControl.Forward().x * 1000.0f;
						m_Velocity.z = m_matControl.Forward().z * 1000.0f;
					}
				}
			}
		}
		if (LInput::GetInstance().m_KeyStateOld[DIK_S] > KEY_PUSH)
		{
			if (IsSteamPack)
			{
				if (LInput::GetInstance().m_KeyStateOld[DIK_A] > KEY_PUSH)
				{
					m_Velocity.x = (m_matControl.Forward().x + m_matControl.Right().x) * -1000.0f;
					m_Velocity.z = (m_matControl.Forward().z + m_matControl.Right().z) * -1000.0f;
				}
				else if (LInput::GetInstance().m_KeyStateOld[DIK_D] > KEY_PUSH)
				{
					m_Velocity.x = (m_matControl.Forward().x - m_matControl.Right().x) * -1000.0f;
					m_Velocity.z = (m_matControl.Forward().z - m_matControl.Right().z) * -1000.0f;
				}
				else
				{
					m_Velocity.x = m_matControl.Forward().x * -1000.0f;
					m_Velocity.z = m_matControl.Forward().z * -1000.0f;
				}
			}
			else
			{
				if (LInput::GetInstance().m_KeyStateOld[DIK_A] > KEY_PUSH)
				{
					m_Velocity.x = (m_matControl.Forward().x - m_matControl.Right().x) * -500.0f;
					m_Velocity.z = (m_matControl.Forward().z - m_matControl.Right().z) * -500.0f;
				}
				else if (LInput::GetInstance().m_KeyStateOld[DIK_D] > KEY_PUSH)
				{
					m_Velocity.x = (m_matControl.Forward().x + m_matControl.Right().x) * -500.0f;
					m_Velocity.z = (m_matControl.Forward().z + m_matControl.Right().z) * -500.0f;
				}
				else
				{
					m_Velocity.x = m_matControl.Forward().x * -500.0f;
					m_Velocity.z = m_matControl.Forward().z * -500.0f;
				}
			}
		}
		IsOnAir = true;
	}
}

void LPlayer::ItemChnge(GunState gun, std::wstring gunName, int gunIndex)
{
	m_CurrentGun = gun;
	m_Gun->m_pModel = LFbxMgr::GetInstance().GetPtr(gunName);

	if (m_Gun->m_pModel == nullptr) return;

	m_Gun->m_ParentBoneName = LExportIO::GetInstance().m_ItemParentName[0];
	m_Gun->m_pModel->m_matScale = LExportIO::GetInstance().m_ItemScale[0];
	m_Gun->m_pModel->m_matRotation = LExportIO::GetInstance().m_ItemRotation[0];
	m_Gun->m_pModel->m_matTranslation = LExportIO::GetInstance().m_ItemTranslation[0];
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
		LGlobal::g_PlayerHitSound->PlayEffect();
		if (m_HP <= 61.0f)
		{
			UIManager::GetInstance().GetUIObject(L"face")->GetScript<ChangeTexture>(L"ChangeTexture")->ChangeFromPath(L"../../res/ui/face2.png");
		}
		if (m_HP <= 21.0f)
		{
			UIManager::GetInstance().GetUIObject(L"face")->GetScript<ChangeTexture>(L"ChangeTexture")->ChangeFromPath(L"../../res/ui/face3.png");
		}
	}

	if (LInput::GetInstance().m_KeyStateOld[DIK_4] == KEY_PUSH)
	{
		ItemChnge(GunState::PISTOL, L"Pistols_A.fbx", 0);
	}
	else if (LInput::GetInstance().m_KeyStateOld[DIK_5] == KEY_PUSH)
	{
		ItemChnge(GunState::ASSAULTRIFLE, L"Assault_Rifle_A.fbx", 1);
	}
	
	m_Gun->m_StartShoot += LGlobal::g_fSPF;
	IsShoot = false;
	IsReload = false;

	if ((LInput::GetInstance().m_MouseState[0] > KEY_PUSH) && IsEndReload && abs(accumulatedTime < 0.001f))
	{
		IsMove = true;
		IsAttack = true;

		if (m_Gun->m_StartShoot > m_Gun->m_ShotDelay)
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

			m_Gun->m_StartShoot = 0.0f;
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
		LGlobal::g_SteamPackSound->PlayEffect();
		UIManager::GetInstance().GetUIObject(L"HPbar")->GetScript<HpBar>(L"HpBar")->UpdateHp();
	}



	if (LInput::GetInstance().m_KeyStateOld[DIK_Q] == KEY_PUSH)
	{
		m_HP = min(100, m_HP + 30);
		
		UIManager::GetInstance().GetUIObject(L"HPbar")->GetScript<HpBar>(L"HpBar")->UpdateHp();
	}

	if (IsSteamPack)
	{
		m_SteamPackStart += LGlobal::g_fSPF;
		m_Gun->m_ShotDelay = 0.05f;
		m_AnimationRate = 1.5f;
		float excleSpeed = m_Speed * 1.5;
		m_Speed = excleSpeed;
	}

	if (m_SteamPackEnd < m_SteamPackStart)
	{
		m_Gun->m_ShotDelay = 0.1f;
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
	if (LInput::GetInstance().m_KeyStateOld[DIK_Z] > KEY_PUSH)
	{
		m_ZedTimeCount = 1;
		IsZedTime = true;
		LGlobal::g_ZedTimeStart->PlayEffect();
	}


	if (IsZedTime)
	{
		m_ZedTimeStart += LGlobal::g_fSPF;
		/*m_ShotDelay = 0.05f;
		m_AnimationRate = 2.0f;
		float excleSpeed = m_Speed * 2;
		m_Speed = excleSpeed;*/
	}

	if (m_ZedTimeEnd < m_ZedTimeStart)
	{
		m_ZedTimeStart = 0.0f;
		m_AnimationRate = 1.0f;
		IsZedTime = false;
		LGlobal::g_ZedTimeEnd->PlayEffect();
	}

	if (IsMove)
	{
		Move();
	}

	if (LInput::GetInstance().m_KeyStateOld[DIK_LSHIFT] > KEY_PUSH && LInput::GetInstance().m_KeyStateOld[DIK_W] > KEY_PUSH)
	{
		m_Speed = RUNMOVESPEED;
	}
	//else if (LInput::GetInstance().m_KeyStateOld[DIK_LSHIFT] > KEY_PUSH && LInput::GetInstance().m_KeyStateOld[DIK_S] > KEY_PUSH)
	//{
	//	m_Speed = 100.0f;
	//}

	if (LInput::GetInstance().m_KeyStateOld[DIK_LSHIFT] == KEY_UP)
	{
		m_Speed = 0.0f;
	}

	if (m_Speed > 100.0f)
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

//LPlayer::LPlayer()
//{
//	m_GunModel = std::make_shared<LModel>();
//}