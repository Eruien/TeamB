#pragma once
#include "LModel.h"
#include "LInput.h"
#include "LFSMMgr.h"
#include "LPlayer.h"
#include "LSoundMgr.h"
#include <random>
#include <chrono>

class Tank;
class KObject;

class TankState
{
public:
	Tank* m_pOwner = nullptr;
public:
	virtual bool Init() { return true; }
	virtual	void Process() = 0;
public:
	TankState(Tank* parent) : m_pOwner(parent) {}
	virtual ~TankState() {}
};

class Tank : public LSkinningModel
{
public:
	State m_CurrentState = State::NONE;
	LFiniteStateMachine* m_pFsm = nullptr;
	TankState* m_pAction = nullptr;
	std::map<State, std::unique_ptr<TankState>> m_pActionList;
public:
	bool IsDead = false;
	bool IsTakeDamage = false;
	bool IsComboRange = false;
	bool IsMovable = true;
	bool IsRush = false;
	bool IsRushDir = true;
	bool IsUseRush = true;
	LSound* m_ZombieSound = nullptr;
	float m_Speed = 50.0f;
	float m_RushSpeed = 400.0f;
	float m_ComboSpeed = 200.0f;
	TVector3 m_Dir;
	float m_ComboRange = 50.0f;
	float m_RushRange = 400.0f;
	float m_RushStart = 0.0f;
	float m_RushEnd = 1.0f;
	float m_RushCoolTimeStart = 0.0f;
	float m_RushCoolTimeEnd = 30.0f;
	TVector3 m_RandomPos;
	LPlayer* m_Player;
	TVector3 m_PlayerPos;
	TVector3 m_NPCPos;
	TVector3 m_RushPos;
	std::default_random_engine m_Generator;
	std::uniform_int_distribution<int> m_Distribution;
	shared_ptr<KObject> m_enemyHp;
	shared_ptr<KObject> m_minimapMarker;
public:
	State GetState();
	void FSM(FSMType fsmType);
	void SetTransition(Event inputEvent);
	void Move(TVector3 target);
	void RushMove();
	void ComboMove();
	int GetRandomNumber();
public:
	virtual	void Process();
	bool Frame() override;
	bool Render() override;
	bool RenderMark();
public:
	Tank() {};
	Tank(LPlayer* player);
};

