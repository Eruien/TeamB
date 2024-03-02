#pragma once
#include "LModel.h"
#include "LInput.h"
#include "LFSMMgr.h"

class LPlayer;

class PlayerState
{
public:
	LPlayer* m_pOwner = nullptr;
public:
	virtual bool Init() { return true; }
	virtual	void Process() = 0;
public:
	PlayerState(LPlayer* parent) : m_pOwner(parent) {}
	virtual ~PlayerState() {}
};

class LPlayer : public LSkinningModel
{
public:
	State m_CurrentState = State::NONE;
	LFiniteStateMachine* m_pFsm = nullptr;
	PlayerState* m_pAction = nullptr;
	std::map<State, std::unique_ptr<PlayerState>> m_pActionList;
public:
	bool IsWalk = false;
	bool IsRun = false;
	bool IsAttack = false;
	bool IsMove = true;
	bool IsMovable = true;
	bool IsReload = false;
	bool IsEndReload = true;
	bool IsShoot = false;
	bool IsMoveOneDir = false;
	bool IsTakeDamage = false;
	bool IsInvincibility = true;
	bool IsDeath = false;
	bool IsTakeDammageAni = false;
	bool IsSteamPack = false;
	bool IsZedTime = false;
	int m_ZedTimeCount = 1;
	float m_Speed = 0.0f;
	float m_ShotDelay = 0.1f;
	float m_StartShoot = 0.0f;
	float m_StartTakeDamage = 0.0f;
	float m_EndTakeDamage = 1.0f;
	float m_SteamPackStart = 0.0f;
	float m_SteamPackEnd = 5.0f;
	float m_ZedTimeStart = 0.0f;
	float m_ZedTimeEnd = 5.0f;
	TVector3 m_AddDirection;

	// 충돌 처리를 위한 직전 위치. 시진 추가
	TVector3 m_PrevPosition;
public:
	void FSM(FSMType fsmType);
	void SetTransition(Event inputEvent);
	State GetState();
	void Move();
public:
	virtual	void Process();
	bool Frame() override;
};

