#pragma once
#include <map>
#include <memory>

enum class State
{
	NONE,

	//Scene
	MAINSCENE,
	UISCENE,
	MAPSCENE,
	CHARACTERSCENE,
	EFFECTSCENE,
};

enum class Event
{
	//Scene
	GOMAINSCENE,
	GOUISCENE,
	GOMAPSCENE,
	GOCHARACTERSCENE,
	GOEFFECTSCENE,
};

class LFiniteState
{
private:
	State m_State;
public:
	std::map<Event, State> m_StateMap;
public:
	State GetState() { return m_State; }
public:
	void AddTransition(Event inputEvent, State outputState);
	State Output(Event inputEvent);
public:
	LFiniteState(State state);
};

