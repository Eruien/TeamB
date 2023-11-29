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

// ����Ʈ ������ �����ؼ� ������ ������ ������ ���� ����Ʈ �����͸� move�� �����ؼ� ������ �Ѱ��ش�.
bool LFSMMgr::Get(FSMType key, LFiniteStateMachine& ret)
{
	auto iter = m_map.find(key);

	if (m_map.end() == iter)
	{
		return false;
	}

	// ������ �ּҰ� �ƴ϶� ��ü�� ���� �ּҰ� �ʿ��ϴϱ� ������ �ؼ� move
	ret = std::move(*iter->second);

	return true;
}

bool LFSMMgr::Init()
{
	//Scene
	std::unique_ptr<LFiniteStateMachine> sceneFSM = std::make_unique<LFiniteStateMachine>();

	sceneFSM->AddStateTransition(State::MAINSCENE, Event::GOUISCENE, State::UISCENE);
	sceneFSM->AddStateTransition(State::MAINSCENE, Event::GOMAPSCENE, State::MAPSCENE);
	sceneFSM->AddStateTransition(State::MAINSCENE, Event::GOCHARACTERSCENE, State::CHARACTERSCENE);
	sceneFSM->AddStateTransition(State::MAINSCENE, Event::GOEFFECTSCENE, State::EFFECTSCENE);

	sceneFSM->AddStateTransition(State::UISCENE, Event::GOMAINSCENE, State::MAINSCENE);
	sceneFSM->AddStateTransition(State::MAPSCENE, Event::GOMAINSCENE, State::MAINSCENE);
	sceneFSM->AddStateTransition(State::CHARACTERSCENE, Event::GOMAINSCENE, State::MAINSCENE);
	sceneFSM->AddStateTransition(State::EFFECTSCENE, Event::GOMAINSCENE, State::MAINSCENE);
	
	m_map.insert(std::make_pair(FSMType::SCENE, std::move(sceneFSM)));

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


