#define DIRECTINPUT_VERSION 0x0800


#pragma once
#include "LStd.h"
// direct input
#include "dinput.h"
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")



enum class KeyState
{
	KEY_FREE = 0,
	KEY_UP = 1,
	KEY_PUSH = 2,
	KEY_HOLD = 3,
};

struct INPUT_MAP
{
	bool bUpKey;
	bool bDownKey;
	bool bLeftKey;
	bool bRightKey;

	bool bWKey;
	bool bSKey;
	bool bAKey;
	bool bDKey;
	bool bQKey;
	bool bZKey;
	bool bCKey;
	
	bool bLeftClick;
	bool bRightClick;
	bool bMiddleClick;

	bool bLeftHold;
	bool bRightHold;
	bool bMiddleHold;

	bool bExit;
	bool bSpace;
	
	int iMouseValue[3];

	bool bFullScreen;
	bool bChangeFillMode;
	bool bChangePrimitive;
	bool bChangeCullMode;
	bool bChangeCameraType;
	bool bDebugRender;

	bool bLShift;
};
extern INPUT_MAP g_InputData; // 전역 구조체로 마우스 상태와 키 상태를 전역으로 저장.


class LInput
{
public:
	static LInput& GetInstance()
	{
		static LInput input;
		return input;
	}

private: // for direct input , [시진]
	IDirectInput8* m_pDI;
	IDirectInputDevice8* m_pKeyDevice;  // 키보드
	IDirectInputDevice8* m_pMouseDevice;// 마우스
	DIMOUSESTATE	m_DIMouseState;
	BYTE	m_MouseState[3];
	BYTE	m_BeforeMouseState[3];
	BYTE	m_KeyState[256];
	bool InitDirectInput();
	bool ShutDownDirectInput();
	BYTE GetKey(BYTE dwKey);
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

