#pragma once
#include "LModel.h"
#include "LInput.h"
#include "LFSMMgr.h"
#include "LPlayer.h"
#include <random>
#include <chrono>

class LNPC;

class NPCState
{
public:
	LNPC* m_pOwner = nullptr;
public:
	virtual bool Init() { return true; }
	virtual	void Process() = 0;
public:
	NPCState(LNPC* parent) : m_pOwner(parent) {}
	virtual ~NPCState() {}
};

class LNPC : public LSkinningModel
{
public:
	State m_CurrentState = State::NONE;
	LFiniteStateMachine* m_pFsm = nullptr;
	NPCState* m_pAction = nullptr;
	std::map<State, std::unique_ptr<NPCState>> m_pActionList;
public:
	bool IsFindPlayer = false;
	bool IsEndPatrol = false;
	bool IsDead = false;
	float m_Speed = 100.0f;
	TVector3 m_Dir;
	float m_PatrolRange = 1000.0f;
	TVector3 m_RandomPos;
	LPlayer* m_Player;
	TVector3 m_PlayerPos;
	TVector3 m_NPCPos;
	std::default_random_engine m_Generator;
	std::uniform_int_distribution<int> m_Distribution;
public:
	void FSM(FSMType fsmType);
	void SetTransition(Event inputEvent);
	State GetState();
	void Move(TVector3 target);
	void MoveInstancing(TVector3 target);
	int GetRandomNumber();
public:
	virtual	void Process();
	bool Frame() override;
	bool FrameInstancing() override;
public:
	LNPC(LPlayer* player);
};

