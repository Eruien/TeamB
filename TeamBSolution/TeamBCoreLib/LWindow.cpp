#include "LWindow.h"
#include "LGlobal.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

bool LWindow::SetRegisterWindowClass(HINSTANCE hInstance)
{
    m_hInstance = hInstance;
    
    WNDCLASSEXW wcex = {0,};

    wcex.cbSize = sizeof(WNDCLASSEXW);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(128, 128, 128));
    wcex.hCursor = LoadCursor(nullptr, IDC_CROSS);
    wcex.lpszClassName = L"LWindow";

    WORD ret = RegisterClassExW(&wcex);

    if (!ret)
    {
        MessageBoxA(NULL, "Window Class Register Error", "Error Box", MB_OK);
    }
  
    return true;
}

bool LWindow::SetCreateWindow(LPCWSTR lpWindowName, int WindowWidth, int WindowHeight)
{
    LGlobal::g_WindowWidth = m_WindowWidth = WindowWidth;
    LGlobal::g_WindowHeight = m_WindowHeight = WindowHeight;

    m_hWnd = CreateWindowExW(
        m_dwExStyle,
        L"LWindow",
        lpWindowName,
        m_dwStyle,
        m_WindowPosX,
        m_WindowPosY,
        m_WindowWidth,
        m_WindowHeight,
        nullptr,
        nullptr,
        m_hInstance,
        nullptr);

    if (!m_hWnd)
    {
        MessageBoxA(NULL, "Create Window Error", "Error Box", MB_OK);
    }

    LGlobal::g_hWnd = m_hWnd;
    ShowWindow(m_hWnd, SW_SHOW);

    return true;
}

LWindow::~LWindow() {}