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
	CHARACTERTAKEDAMAGE,
	CHARACTERDEATH,

	//Scene
	MAINSCENE,
	INGAMESCENE,
	ENDSCENE,
	SHOPSCENE,

	//Enemy
	ENEMYPATROL,
	ENEMYTRACE,
	ENEMYDEATH,
	ENEMYTAKEDAMAGE,
	ENEMYATTACK,

	//Tank
	TANKTRACE,
	TANKDEATH,
	TANKTAKEDAMAGE,
	TANKRUSH,
	TANKCOMBO,
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
	GOSHOPSCENE,

	//Enemy
	FINDPLAYER,
	FATALDAMAGE,
	TAKEDAMAGE,
	RECOVERYDAMAGE,
	PLAYERINATTACKRANGE,
	PLAYEROUTATTACKRANGE,

	STARTRELOAD,
	ENDRELOAD,
	CHARACTERREVIVE,

	//Tank
	TANKCOMBOATTACK,
	TANKRUSHATTACK,
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

