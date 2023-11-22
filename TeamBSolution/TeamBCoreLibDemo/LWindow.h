#pragma once
#include"LStd.h"

class LWindow
{
public:
    // 멤버 변수 선언을 하는 이유
    // 인스턴스나 핸들을 자식에서도 이용하기 위해서
    // 또는 다른 클래스에서도 사용하기 위해서 정보를 저장하는 용도
    // 멤버로 가지고 있고 초기값을 넣어주기 위해서
    HINSTANCE m_hInstance;
    HWND m_hWnd;
    DWORD m_dwExStyle = WS_EX_APPWINDOW;
    DWORD m_dwStyle = WS_OVERLAPPEDWINDOW;
    int m_WindowWidth = 400;
    int m_WindowHeight = 300;
    int m_WindowPosX = 1920 / 2 - m_WindowWidth / 2;
    int m_WindowPosY = 1080 / 2 - m_WindowHeight / 2;
public:
	bool SetRegisterWindowClass(HINSTANCE hInstance);
	bool SetCreateWindow(LPCWSTR lpWindowName = L"기본창", int WindowWidth = 400, int WindowHeight = 300);
public:
    virtual ~LWindow();
};

