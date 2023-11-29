#pragma once
#include "LStd.h"

enum class KeyState
{
	KEY_FREE = 0,
	KEY_UP = 1,
	KEY_PUSH = 2,
	KEY_HOLD = 3,
};

class LInput
{
public:
	static LInput& GetInstance()
	{
		static LInput input;
		return input;
	}
public:
	TVector3 m_vOffset = { 0,0,0 };
	POINT m_MousePos;
	POINT m_BeforeMousePos;
	DWORD m_dwKeyState[256] = {0,};
public:
	TVector3 GetWorldPos(float windowWidth, float windowHeight, float cameraPosX, float cameraPosY);
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
private:
	LInput() {}
public:
	~LInput() {}
};

