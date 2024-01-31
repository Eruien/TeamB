#pragma once
#include <map>
#include <memory>

enum class State
{
	NONE,
	// Character
	CHARACTERIDLE,
	CHARACTERWALK,
	CHARACTERRUN,
	CHARACTERSHOOT,
	CHARACTERRELOAD,

	//Scene
	MAINSCENE,
	INGAMESCENE,
	ENDSCENE,

	//Enemy
	ENEMYPATROL,
	ENEMYTRACE,
	ENEMYDEATH,
	ENEMYTAKEDAMAGE,
	ENEMYATTACK,
};

enum class Event
{
	//Character
	IDLESPEED,
	WALKSPEED,
	RUNSPEED,
	CLICKATTACKBUTTON,
	ENDATTACK,

	//Scene
	GOMAINSCENE,
	GOINGAMESCENE,
	GOENDSCENE,

	//Enemy
	FINDPLAYER,
	FATALDAMAGE,
	TAKEDAMAGE,
	RECOVERYDAMAGE,
	PLAYERINATTACKRANGE,
	PLAYEROUTATTACKRANGE,

	STARTRELOAD,
	ENDRELOAD,
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

