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

	//Scene
	MAINSCENE,
	UISCENE,
	MAPSCENE,
	CHARACTERSCENE,
	EFFECTSCENE,

	//Enemy
	ENEMYPATROL,
	ENEMYTRACE,
	ENEMYDEATH,
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
	GOUISCENE,
	GOMAPSCENE,
	GOCHARACTERSCENE,
	GOEFFECTSCENE,

	//Enemy
	FINDPLAYER,
	FATALDAMAGE,
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

