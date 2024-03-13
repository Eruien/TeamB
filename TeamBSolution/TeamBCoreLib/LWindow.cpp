#include "LWindow.h"
#include "LGlobal.h"

//


#include <new>
#include <windows.h>
#include <windowsx.h>
#include <mfplay.h>
#include <mferror.h>
#include <shobjidl.h>   // defines IFileOpenDialog
#include <strsafe.h>
#include <sstream>
#include <mfapi.h>

#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mfuuid.lib")
#pragma comment(lib, "mfreadwrite.lib")
#pragma comment(lib, "mf.lib")
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "mfplay.lib")
#include "resource.h"


#include <Shlwapi.h>

class MediaPlayerCallback : public IMFPMediaPlayerCallback
{
    long m_cRef; // Reference count

public:

    MediaPlayerCallback() : m_cRef(1)
    {
    }

    STDMETHODIMP QueryInterface(REFIID riid, void** ppv)
    {
        static const QITAB qit[] =
        {
            QITABENT(MediaPlayerCallback, IMFPMediaPlayerCallback),
            { 0 },
        };
        return QISearch(this, qit, riid, ppv);
    }
    STDMETHODIMP_(ULONG) AddRef()
    {
        return InterlockedIncrement(&m_cRef);
    }
    STDMETHODIMP_(ULONG) Release()
    {
        ULONG count = InterlockedDecrement(&m_cRef);
        if (count == 0)
        {
            delete this;
            return 0;
        }
        return count;
    }

    // IMFPMediaPlayerCallback methods
    void STDMETHODCALLTYPE OnMediaPlayerEvent(MFP_EVENT_HEADER* pEventHeader);
};

// Include the v6 common controls in the manifest
#pragma comment(linker, \
    "\"/manifestdependency:type='Win32' "\
    "name='Microsoft.Windows.Common-Controls' "\
    "version='6.0.0.0' "\
    "processorArchitecture='*' "\
    "publicKeyToken='6595b64144ccf1df' "\
    "language='*'\"")


HRESULT PlayMediaFile(HWND hwnd, const WCHAR* sURL);
void    ShowErrorMessage(PCWSTR format, HRESULT hr);

// Window message handlers
void    OnClose(HWND hwnd);
void    OnPaint(HWND hwnd);
void    OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void    OnSize(HWND hwnd, UINT state, int cx, int cy);
void    OnKeyDown(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags);

// load and play a video
//void startPlayer(HWND hwnd, const WCHAR* pwszFilePath);

// Menu handlers
void    OnFileOpen(HWND hwnd);

// MFPlay event handler functions.
void OnMediaItemCreated(MFP_MEDIAITEM_CREATED_EVENT* pEvent);
void OnMediaItemSet(MFP_MEDIAITEM_SET_EVENT* pEvent);
void OnPlayEnded(MFP_PLAYBACK_ENDED_EVENT* pEvent);

// Constants 
const WCHAR CLASS_NAME[] = L"MFPlay Window Class";
const WCHAR WINDOW_NAME[] = L"MFPlay Sample Application";
// Global variables

IMFPMediaPlayer* g_pPlayer = nullptr;      // The MFPlay player object.
IMFVideoDisplayControl* pVideoDisplayControl = nullptr;
IMFMediaSession* pSession = NULL;
MediaPlayerCallback* g_pPlayerCB = nullptr;    // Application callback object.
const wchar_t* path;
BOOL                    g_bHasVideo = FALSE;

/////////////////////////////////////////////////////////////////////

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT hr = LGlobal::g_pWindow->MsgProc(hWnd, message, wParam, lParam);
    if (SUCCEEDED(hr)) return 0;

    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;
 
    switch (message)
    {
        HANDLE_MSG(hWnd, WM_PAINT, OnPaint);
        HANDLE_MSG(hWnd, WM_KEYDOWN, OnKeyDown);
        
        
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        
    }
  
    return 0;
}

int LWindow::MsgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_SIZE:
        if (SIZE_MINIMIZED != wParam)
        {
            UINT width = LOWORD(lParam);
            UINT height = HIWORD(lParam);
            
            ResizeDevice(width, height);
        }
        break;
    }

    return -1;
}

bool LWindow::SetRegisterWindowClass(HINSTANCE hInstance)
{
    m_hInstance = hInstance;

    WNDCLASSEXW wcex = { 0, };

    wcex.cbSize = sizeof(WNDCLASSEXW);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(128, 128, 128));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
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
    RECT rc = { 0,0, WindowWidth, WindowHeight };

    AdjustWindowRect(&rc, m_dwStyle, FALSE);

    m_hWnd = CreateWindowExW(
        m_dwExStyle,
        L"LWindow",
        lpWindowName,
        m_dwStyle,
        m_WindowPosX,
        m_WindowPosY,
        rc.right - rc.left,
        rc.bottom - rc.top,
        nullptr,
        nullptr,
        m_hInstance,
        nullptr);

    if (!m_hWnd)
    {
        MessageBoxA(NULL, "Create Window Error", "Error Box", MB_OK);
    }

    LGlobal::g_hWnd = m_hWnd;

    GetClientRect(m_hWnd, &m_rcClient);
    LGlobal::g_WindowWidth = m_WindowWidth = m_rcClient.right;
    LGlobal::g_WindowHeight = m_WindowHeight = m_rcClient.bottom;
    int x = 100;
    int y = 100;
    SetWindowPos(m_hWnd, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
    ShowWindow(m_hWnd, SW_SHOW);

    return true;
}

LWindow::LWindow()
{
    LGlobal::g_pWindow = this;
}

LWindow::~LWindow() {}



//-------------------------------------------------------------------
// OnClose
//
// Handles the WM_CLOSE message.
//-------------------------------------------------------------------

void OnClose(HWND /*hwnd*/)
{
    if (g_pPlayer)
    {
        g_pPlayer->Shutdown();
        g_pPlayer->Release();
        g_pPlayer = NULL;
    }

    if (g_pPlayerCB)
    {
        g_pPlayerCB->Release();
        g_pPlayerCB = NULL;
    }

    PostQuitMessage(0);
}


//-------------------------------------------------------------------
// OnPaint
//
// Handles the WM_PAINT message.
//-------------------------------------------------------------------

void OnPaint(HWND hwnd)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    if (g_pPlayer && g_bHasVideo)
    {
        // Playback has started and there is video. 

        // Do not draw the window background, because the video 
        // frame fills the entire client area.
        
        g_pPlayer->UpdateVideo();
      
    }
    else
    {
        // There is no video stream, or playback has not started, or video has ended.
        // Paint the entire client area with the background color.
       
        FillRect(hdc, &ps.rcPaint, CreateSolidBrush(RGB(255, 0, 0)));
    }

    EndPaint(hwnd, &ps);
}


//-------------------------------------------------------------------
// OnSize
//
// Handles the WM_SIZE message.
//-------------------------------------------------------------------

void OnSize(HWND /*hwnd*/, UINT state, int /*cx*/, int /*cy*/)
{
    if (state == SIZE_RESTORED)
    {
        if (g_pPlayer)
        {
            // Resize the video.
            g_pPlayer->UpdateVideo();
        }
    }
}


//-------------------------------------------------------------------
// OnKeyDown
//
// Handles the WM_KEYDOWN message.
//-------------------------------------------------------------------

void OnKeyDown(HWND /*hwnd*/, UINT vk, BOOL /*fDown*/, int /*cRepeat*/, UINT /*flags*/)
{
    HRESULT hr = S_OK;

    switch (vk)
    {
    case VK_SPACE:

        // Toggle between playback and paused/stopped.
        if (g_pPlayer)
        {
            MFP_MEDIAPLAYER_STATE state = MFP_MEDIAPLAYER_STATE_EMPTY;

            hr = g_pPlayer->GetState(&state);

            if (SUCCEEDED(hr))
            {
                
                g_pPlayer->Shutdown();
                
                //if (state == MFP_MEDIAPLAYER_STATE_PAUSED || state == MFP_MEDIAPLAYER_STATE_STOPPED)
                //{
                //    hr = g_pPlayer->Play();
                //    //g_pPlayer->Shutdown();
                //}
                //else if (state == MFP_MEDIAPLAYER_STATE_PLAYING)
                //{
                //    hr = g_pPlayer->Pause();
                //}
            }
        }
        break;
    }

    if (FAILED(hr))
    {
        ShowErrorMessage(TEXT("Playback Error"), hr);
    }
}


//-------------------------------------------------------------------
// OnCommand
// 
// Handles the WM_COMMAND message.
//-------------------------------------------------------------------

void OnCommand(HWND hwnd, int id, HWND /*hwndCtl*/, UINT /*codeNotify*/)
{
    switch (id)
    {
    case ID_FILE_OPEN:
        OnFileOpen(hwnd);
        break;

    case ID_FILE_EXIT:
        OnClose(hwnd);
        break;
    }
}

/*
  video player for cvpr2024
*/

//-------------------------------------------------------------------
// OnFileOpen
//
// Handles the "File Open" command.
//-------------------------------------------------------------------

void LWindow::startPlayer(HWND hwnd, const WCHAR* pwszFilePath)
{
    HRESULT hr = S_OK;

    // Open the media file.
    hr = PlayMediaFile(hwnd, pwszFilePath);

    if (FAILED(hr)) { goto done; }

done:
    if (FAILED(hr))
    {
        ShowErrorMessage(L"Could not open file.", hr);
    }

}




//-------------------------------------------------------------------
// OnFileOpen
//
// Handles the "File Open" command.
//-------------------------------------------------------------------

void OnFileOpen(HWND hwnd)
{
    HRESULT hr = S_OK;

    IFileOpenDialog* pFileOpen = NULL;
    IShellItem* pItem = NULL;

    PWSTR pwszFilePath = NULL;

    // Create the FileOpenDialog object.
    hr = CoCreateInstance(
        __uuidof(FileOpenDialog),
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&pFileOpen)
    );

    if (FAILED(hr)) { goto done; }


    hr = pFileOpen->SetTitle(L"Select a File to Play");

    if (FAILED(hr)) { goto done; }


    // Show the file-open dialog.
    hr = pFileOpen->Show(hwnd);

    if (hr == HRESULT_FROM_WIN32(ERROR_CANCELLED))
    {
        // User cancelled.
        hr = S_OK;
        goto done;
    }
    if (FAILED(hr)) { goto done; }


    // Get the file name from the dialog.
    hr = pFileOpen->GetResult(&pItem);

    if (FAILED(hr)) { goto done; }


    hr = pItem->GetDisplayName(SIGDN_URL, &pwszFilePath);

    if (FAILED(hr)) { goto done; }


    // Open the media file.
    hr = PlayMediaFile(hwnd, pwszFilePath);

    if (FAILED(hr)) { goto done; }

done:
    if (FAILED(hr))
    {
        ShowErrorMessage(L"Could not open file.", hr);
    }

    CoTaskMemFree(pwszFilePath);

    if (pItem)
    {
        pItem->Release();
    }
    if (pFileOpen)
    {
        pFileOpen->Release();
    }
}


//-------------------------------------------------------------------
// PlayMediaFile
//
// Plays a media file, using the IMFPMediaPlayer interface.
//-------------------------------------------------------------------

HRESULT PlayMediaFile(HWND hwnd, const WCHAR* sURL)
{
    HRESULT hr = S_OK;

    // Create the MFPlayer object.
    if (g_pPlayer == NULL)
    {
        g_pPlayerCB = new (std::nothrow) MediaPlayerCallback();

        if (g_pPlayerCB == NULL)
        {
            hr = E_OUTOFMEMORY;
            goto done;
        }

        hr = MFPCreateMediaPlayer(
            NULL,
            FALSE,          // Start playback automatically?
            0,              // Flags
            g_pPlayerCB,    // Callback pointer
            hwnd,           // Video window
            &g_pPlayer
        );
        
        if (FAILED(hr)) { goto done; }
    }

    // Create a new media item for this URL.
    hr = g_pPlayer->CreateMediaItemFromURL(sURL, FALSE, 0, NULL);

    // The CreateMediaItemFromURL method completes asynchronously. 
    // The application will receive an MFP_EVENT_TYPE_MEDIAITEM_CREATED 
    // event. See MediaPlayerCallback::OnMediaPlayerEvent().


done:
    return hr;
}


//-------------------------------------------------------------------
// OnMediaPlayerEvent
// 
// Implements IMFPMediaPlayerCallback::OnMediaPlayerEvent.
// This callback method handles events from the MFPlay object.
//-------------------------------------------------------------------

void MediaPlayerCallback::OnMediaPlayerEvent(MFP_EVENT_HEADER* pEventHeader)
{
    if (FAILED(pEventHeader->hrEvent))
    {
        ShowErrorMessage(L"Playback error", pEventHeader->hrEvent);
        return;
    }

    switch (pEventHeader->eEventType)
    {
    case MFP_EVENT_TYPE_MEDIAITEM_CREATED:
        OnMediaItemCreated(MFP_GET_MEDIAITEM_CREATED_EVENT(pEventHeader));
        break;

    case MFP_EVENT_TYPE_MEDIAITEM_SET:
        OnMediaItemSet(MFP_GET_MEDIAITEM_SET_EVENT(pEventHeader));
        break;

    case MFP_EVENT_TYPE_PLAYBACK_ENDED:
        OnPlayEnded(MFP_GET_PLAYBACK_ENDED_EVENT(pEventHeader));
        break;
    }
}


//-------------------------------------------------------------------
// OnMediaItemCreated
//
// Called when the IMFPMediaPlayer::CreateMediaItemFromURL method
// completes.
//-------------------------------------------------------------------

void OnMediaItemCreated(MFP_MEDIAITEM_CREATED_EVENT* pEvent)
{
    HRESULT hr = S_OK;

    // The media item was created successfully.

    if (g_pPlayer)
    {
        BOOL bHasVideo = FALSE, bIsSelected = FALSE;

        // Check if the media item contains video.
        hr = pEvent->pMediaItem->HasVideo(&bHasVideo, &bIsSelected);

        if (FAILED(hr)) { goto done; }

        g_bHasVideo = bHasVideo && bIsSelected;

        // Set the media item on the player. This method completes asynchronously.
        hr = g_pPlayer->SetMediaItem(pEvent->pMediaItem);
    }

done:
    if (FAILED(hr))
    {
        ShowErrorMessage(L"Error playing this file.", hr);
    }
}


//-------------------------------------------------------------------
// OnMediaItemSet
//
// Called when the IMFPMediaPlayer::SetMediaItem method completes.
//-------------------------------------------------------------------

void OnMediaItemSet(MFP_MEDIAITEM_SET_EVENT* /*pEvent*/)
{
    HRESULT hr = S_OK;

    hr = g_pPlayer->Play();

    if (FAILED(hr))
    {
        ShowErrorMessage(L"IMFPMediaPlayer::Play failed.", hr);
    }
}


//-------------------------------------------------------------------
// OnPlayEnded
//
// Called when the IMFPMediaPlayer::SetMediaItem method completes.
//-------------------------------------------------------------------

void OnPlayEnded(MFP_PLAYBACK_ENDED_EVENT* /*pEvent*/)
{
    HRESULT hr = S_OK;
    if (g_pPlayer)
    {
        //g_pPlayer->Shutdown();
        g_pPlayer->Release();
        g_pPlayer = NULL;
    }

    if (g_pPlayerCB)
    {
        g_pPlayerCB->Release();
        g_pPlayerCB = NULL;
    }
    //MFShutdown();
    if (FAILED(hr))
    {
        ShowErrorMessage(L"IMFPMediaPlayer::OnPlayEnded failed.", hr);
    }
}


void ShowErrorMessage(PCWSTR format, HRESULT hrErr)
{
    HRESULT hr = S_OK;
    WCHAR msg[MAX_PATH];

    hr = StringCbPrintf(msg, sizeof(msg), L"%s (hr=0x%X)", format, hrErr);

    if (SUCCEEDED(hr))
    {
        MessageBox(NULL, msg, L"Error", MB_ICONERROR);
    }
}
