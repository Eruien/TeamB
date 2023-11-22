#include "LFSMMgr.h"

void LFiniteStateMachine::AddStateTransition(State keyState, Event inputEvent, State outputState)
{
	auto finiteIter = m_FiniteStateMap.find(keyState);

	if (m_FiniteStateMap.end() != finiteIter)
	{
		finiteIter->second->AddTransition(inputEvent, outputState);
	}
	else
	{
		m_FiniteStateMap.insert(std::make_pair(keyState, std::make_unique<LFiniteState>(keyState)));

		finiteIter = m_FiniteStateMap.find(keyState);
		finiteIter->second->AddTransition(inputEvent, outputState);
	}
}

State LFiniteStateMachine::StateTransition(State currentState, Event inputEvent)
{
	auto finiteIter = m_FiniteStateMap.find(currentState);

	if (m_FiniteStateMap.end() == finiteIter)
	{
		return State::NONE;
	}

	auto retStateIter = finiteIter->second->m_StateMap.find(inputEvent);
	State state = retStateIter->second;

	return state;
}

// FSMMgr
LFiniteStateMachine* LFSMMgr::GetPtr(FSMType key)
{
	auto iter = m_map.find(key);

	if (m_map.end() == iter)
	{
		return nullptr;
	}

	return iter->second.get();
}

// 스마트 포인터 관련해서 오류날 소지가 있으니 조심 스마트 포인터를 move로 이전해서 참조로 넘겨준다.
bool LFSMMgr::Get(FSMType key, LFiniteStateMachine& ret)
{
	auto iter = m_map.find(key);

	if (m_map.end() == iter)
	{
		return false;
	}

	// 포인터 주소가 아니라 객체에 대한 주소가 필요하니깐 역참조 해서 move
	ret = std::move(*iter->second);

	return true;
}

bool LFSMMgr::Init()
{
	std::unique_ptr<LFiniteStateMachine> playerFSM = std::make_unique<LFiniteStateMachine>();

	playerFSM->AddStateTransition(State::TAKEDAMAGE, Event::ENDROLLING, State::IDLE);
	playerFSM->AddStateTransition(State::TAKEDAMAGE, Event::HURT, State::USERDEAD);

	playerFSM->AddStateTransition(State::IDLE, Event::HURT, State::TAKEDAMAGE);
	playerFSM->AddStateTransition(State::IDLE, Event::INPUTLR, State::WALK);
	playerFSM->AddStateTransition(State::IDLE, Event::RUNTIME, State::RUN);
	playerFSM->AddStateTransition(State::IDLE, Event::INPUTDOWN, State::CROUCH);
	playerFSM->AddStateTransition(State::IDLE, Event::INPUTUP, State::AIRINHALE);
	playerFSM->AddStateTransition(State::IDLE, Event::INPUTATTACK, State::FIRSTINHALE);
	
	playerFSM->AddStateTransition(State::WALK, Event::HURT, State::TAKEDAMAGE);
	playerFSM->AddStateTransition(State::WALK, Event::KEYUP, State::IDLE);

	playerFSM->AddStateTransition(State::CROUCH, Event::HURT, State::TAKEDAMAGE);
	playerFSM->AddStateTransition(State::CROUCH, Event::KEYUP, State::IDLE);
	playerFSM->AddStateTransition(State::CROUCH, Event::INPUTATTACK, State::SILDE);

	playerFSM->AddStateTransition(State::SILDE, Event::HURT, State::TAKEDAMAGE);
	playerFSM->AddStateTransition(State::SILDE, Event::ENDSLIDETIME, State::IDLE);

	playerFSM->AddStateTransition(State::RUN, Event::HURT, State::TAKEDAMAGE);
	playerFSM->AddStateTransition(State::RUN, Event::KEYUP, State::IDLE);

	playerFSM->AddStateTransition(State::AIRINHALE, Event::HURT, State::TAKEDAMAGE);
	playerFSM->AddStateTransition(State::AIRINHALE, Event::CHANGEFLY, State::FLY);

	playerFSM->AddStateTransition(State::FLY, Event::HURT, State::TAKEDAMAGE);
	playerFSM->AddStateTransition(State::FLY, Event::INPUTUP, State::FLAPPING);
	playerFSM->AddStateTransition(State::FLY, Event::INPUTATTACK, State::AIROUTHALE);

	playerFSM->AddStateTransition(State::FLAPPING, Event::HURT, State::TAKEDAMAGE);
	playerFSM->AddStateTransition(State::FLAPPING, Event::INPUTATTACK, State::AIROUTHALE);
	playerFSM->AddStateTransition(State::FLAPPING, Event::KEYUP, State::FLY);

	playerFSM->AddStateTransition(State::AIROUTHALE, Event::HURT, State::TAKEDAMAGE);
	playerFSM->AddStateTransition(State::AIROUTHALE, Event::AIROUTENDTIME, State::FALL);

	playerFSM->AddStateTransition(State::FALL, Event::HURT, State::TAKEDAMAGE);
	playerFSM->AddStateTransition(State::FALL, Event::COLLISIONFLOOR, State::IDLE);

	playerFSM->AddStateTransition(State::FIRSTINHALE, Event::HURT, State::TAKEDAMAGE);
	playerFSM->AddStateTransition(State::FIRSTINHALE, Event::ENDINHALE, State::SECONDINHALE);
	playerFSM->AddStateTransition(State::FIRSTINHALE, Event::KEYUP, State::IDLE);
	playerFSM->AddStateTransition(State::FIRSTINHALE, Event::ABSORBMONSTER, State::FATIDLE);

	playerFSM->AddStateTransition(State::SECONDINHALE, Event::HURT, State::TAKEDAMAGE);
	playerFSM->AddStateTransition(State::SECONDINHALE, Event::ENDINHALE, State::IDLE);
	playerFSM->AddStateTransition(State::SECONDINHALE, Event::KEYUP, State::IDLE);
	playerFSM->AddStateTransition(State::SECONDINHALE, Event::ABSORBMONSTER, State::FATIDLE);

	playerFSM->AddStateTransition(State::FATIDLE, Event::HURT, State::TAKEDAMAGE);
	playerFSM->AddStateTransition(State::FATIDLE, Event::INPUTLR, State::FATMODE);
	playerFSM->AddStateTransition(State::FATIDLE, Event::INPUTATTACK, State::IDLE);
	playerFSM->AddStateTransition(State::FATIDLE, Event::INPUTDOWN, State::MODECHANGE);
	playerFSM->AddStateTransition(State::FATIDLE, Event::CHANGEANGEL, State::ANGELIDLE);

	playerFSM->AddStateTransition(State::FATMODE, Event::HURT, State::TAKEDAMAGE);
	playerFSM->AddStateTransition(State::FATMODE, Event::KEYUP, State::FATIDLE);

	playerFSM->AddStateTransition(State::MODECHANGE, Event::CHANGEABILITY, State::BEAMIDLE);

	//BeamFSM
	playerFSM->AddStateTransition(State::BEAMIDLE, Event::HURT, State::TAKEDAMAGE);
	playerFSM->AddStateTransition(State::BEAMIDLE, Event::INPUTLR, State::BEAMWALK);
	playerFSM->AddStateTransition(State::BEAMIDLE, Event::RUNTIME, State::BEAMRUN);
	playerFSM->AddStateTransition(State::BEAMIDLE, Event::INPUTDOWN, State::BEAMCROUCH);
	playerFSM->AddStateTransition(State::BEAMIDLE, Event::INPUTUP, State::BEAMAIRINHALE);
	playerFSM->AddStateTransition(State::BEAMIDLE, Event::INPUTCANCLE, State::IDLE);
	playerFSM->AddStateTransition(State::BEAMIDLE, Event::INPUTATTACK, State::BEAMATTACK);
	
	playerFSM->AddStateTransition(State::BEAMWALK, Event::HURT, State::TAKEDAMAGE);
	playerFSM->AddStateTransition(State::BEAMWALK, Event::KEYUP, State::BEAMIDLE);

	playerFSM->AddStateTransition(State::BEAMCROUCH, Event::HURT, State::TAKEDAMAGE);
	playerFSM->AddStateTransition(State::BEAMCROUCH, Event::KEYUP, State::BEAMIDLE);
	playerFSM->AddStateTransition(State::BEAMCROUCH, Event::INPUTATTACK, State::BEAMSILDE);

	playerFSM->AddStateTransition(State::BEAMSILDE, Event::HURT, State::TAKEDAMAGE);
	playerFSM->AddStateTransition(State::BEAMSILDE, Event::ENDSLIDETIME, State::BEAMIDLE);

	playerFSM->AddStateTransition(State::BEAMRUN, Event::HURT, State::TAKEDAMAGE);
	playerFSM->AddStateTransition(State::BEAMRUN, Event::KEYUP, State::BEAMIDLE);

	playerFSM->AddStateTransition(State::BEAMAIRINHALE, Event::HURT, State::TAKEDAMAGE);
	playerFSM->AddStateTransition(State::BEAMAIRINHALE, Event::CHANGEFLY, State::BEAMFLY);

	playerFSM->AddStateTransition(State::BEAMFLY, Event::HURT, State::TAKEDAMAGE);
	playerFSM->AddStateTransition(State::BEAMFLY, Event::INPUTUP, State::BEAMFLAPPING);
	playerFSM->AddStateTransition(State::BEAMFLY, Event::INPUTATTACK, State::BEAMAIROUTHALE);

	playerFSM->AddStateTransition(State::BEAMFLAPPING, Event::HURT, State::TAKEDAMAGE);
	playerFSM->AddStateTransition(State::BEAMFLAPPING, Event::INPUTATTACK, State::BEAMAIROUTHALE);
	playerFSM->AddStateTransition(State::BEAMFLAPPING, Event::KEYUP, State::BEAMFLY);

	playerFSM->AddStateTransition(State::BEAMAIROUTHALE, Event::HURT, State::TAKEDAMAGE);
	playerFSM->AddStateTransition(State::BEAMAIROUTHALE, Event::AIROUTENDTIME, State::BEAMFALL);

	playerFSM->AddStateTransition(State::BEAMFALL, Event::HURT, State::TAKEDAMAGE);
	playerFSM->AddStateTransition(State::BEAMFALL, Event::COLLISIONFLOOR, State::BEAMIDLE);

	playerFSM->AddStateTransition(State::BEAMATTACK, Event::HURT, State::TAKEDAMAGE);
	playerFSM->AddStateTransition(State::BEAMATTACK, Event::ENDBEAMATTACK, State::BEAMIDLE);

	//AngelFSM
	playerFSM->AddStateTransition(State::ANGELIDLE, Event::INPUTLR, State::ANGELWALK);
	playerFSM->AddStateTransition(State::ANGELIDLE, Event::RUNTIME, State::ANGELRUN);
	playerFSM->AddStateTransition(State::ANGELIDLE, Event::INPUTDOWN, State::ANGELCROUCH);
	playerFSM->AddStateTransition(State::ANGELIDLE, Event::INPUTCANCLE, State::IDLE);
	playerFSM->AddStateTransition(State::ANGELIDLE, Event::INPUTATTACK, State::ANGELATTACK);

	playerFSM->AddStateTransition(State::ANGELWALK, Event::KEYUP, State::ANGELIDLE);

	playerFSM->AddStateTransition(State::ANGELCROUCH, Event::KEYUP, State::ANGELIDLE);
	playerFSM->AddStateTransition(State::ANGELCROUCH, Event::INPUTATTACK, State::ANGELSILDE);

	playerFSM->AddStateTransition(State::ANGELSILDE, Event::ENDSLIDETIME, State::ANGELIDLE);

	playerFSM->AddStateTransition(State::ANGELRUN, Event::KEYUP, State::ANGELIDLE);

	playerFSM->AddStateTransition(State::ANGELFLY, Event::INPUTUP, State::ANGELFLAPPING);
	// 상태 지정해주기
	playerFSM->AddStateTransition(State::ANGELFLY, Event::INPUTATTACK, State::ANGELFLYIDLE);

	playerFSM->AddStateTransition(State::ANGELFLAPPING, Event::INPUTATTACK, State::ANGELFLYIDLE);
	playerFSM->AddStateTransition(State::ANGELFLAPPING, Event::KEYUP, State::ANGELFLY);

	/*playerFSM->AddStateTransition(State::BEAMAIROUTHALE, Event::AIROUTENDTIME, State::BEAMFALL);

	playerFSM->AddStateTransition(State::BEAMFALL, Event::COLLISIONFLOOR, State::BEAMIDLE);

	playerFSM->AddStateTransition(State::BEAMATTACK, Event::ENDBEAMATTACK, State::BEAMIDLE);*/

	m_map.insert(std::make_pair(FSMType::Player, std::move(playerFSM)));

	// MonstetFSM
	std::unique_ptr<LFiniteStateMachine> monsterFSM = std::make_unique<LFiniteStateMachine>();

	monsterFSM->AddStateTransition(State::MOVE, Event::FINDTARGET, State::TRACE);

	monsterFSM->AddStateTransition(State::TRACE, Event::ATTACKPOS, State::ATTACK);
	monsterFSM->AddStateTransition(State::TRACE, Event::LOSTTARGET, State::GOHOME);

	monsterFSM->AddStateTransition(State::GOHOME, Event::FINDTARGET, State::TRACE);
	monsterFSM->AddStateTransition(State::GOHOME, Event::ARRIVESTARTPOS, State::MOVE);

	monsterFSM->AddStateTransition(State::ATTACK, Event::OUTATTACKRANGE, State::TRACE);

	m_map.insert(std::make_pair(FSMType::Monster, std::move(monsterFSM)));

	//Scene
	std::unique_ptr<LFiniteStateMachine> sceneFSM = std::make_unique<LFiniteStateMachine>();

	sceneFSM->AddStateTransition(State::MAINSCENE, Event::GONEXTSCENE, State::GAMESCENE);

	sceneFSM->AddStateTransition(State::GAMESCENE, Event::GOMAINSCENE, State::MAINSCENE);
	sceneFSM->AddStateTransition(State::GAMESCENE, Event::GONEXTSCENE, State::BOSSSCENE);

	sceneFSM->AddStateTransition(State::BOSSSCENE, Event::GOMAINSCENE, State::MAINSCENE);
	sceneFSM->AddStateTransition(State::BOSSSCENE, Event::GONEXTSCENE, State::ENDSCENE);

	sceneFSM->AddStateTransition(State::ENDSCENE, Event::GOMAINSCENE, State::MAINSCENE);

	m_map.insert(std::make_pair(FSMType::SCENE, std::move(sceneFSM)));

	//Boss
	std::unique_ptr<LFiniteStateMachine> bossFSM = std::make_unique<LFiniteStateMachine>();

	bossFSM->AddStateTransition(State::BOSSIDLE, Event::BOSSSTARTMOVE, State::BOSSMOVE);
	bossFSM->AddStateTransition(State::BOSSIDLE, Event::BOSSRAZERATTACK, State::BOSSRAZERREADY);

	bossFSM->AddStateTransition(State::BOSSMOVE, Event::BOSSSTARATTACK, State::BOSSSTAR);

	bossFSM->AddStateTransition(State::BOSSSTAR, Event::BOSSENDPATTERN, State::BOSSIDLE);

	bossFSM->AddStateTransition(State::BOSSRAZERREADY, Event::BOSSENDPATTERN, State::BOSSRAZER);
	bossFSM->AddStateTransition(State::BOSSRAZER, Event::BOSSENDPATTERN, State::BOSSTHICKRAZER);
	bossFSM->AddStateTransition(State::BOSSTHICKRAZER, Event::BOSSENDPATTERN, State::BOSSIDLE);

	bossFSM->AddStateTransition(State::BOSSBOOM, Event::BOSSENDPATTERN, State::BOSSIDLE);

	m_map.insert(std::make_pair(FSMType::BOSS, std::move(bossFSM)));

	return true;
}

bool LFSMMgr::Release()
{
	m_map.clear();

	return true;
}

LFSMMgr::LFSMMgr()
{
	Init();
}
LFSMMgr::~LFSMMgr() {}


