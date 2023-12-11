#include "Sample.h"
#include "LGlobal.h"
#include "LSpriteObj.h"

bool Sample::Init()
{
	m_DebugCamera = std::make_shared<LDebugCamera>();
	m_DebugCamera->CreateLookAt({ 0.0f, 200.0f, -100.0f }, { 0.0f, 0.0f, 1.0f });
	m_DebugCamera->CreatePerspectiveFov(L_PI * 0.25, (float)LGlobal::g_WindowWidth / (float)LGlobal::g_WindowHeight, 1.0f, 10000.0f);
	LGlobal::g_pMainCamera = m_DebugCamera.get();

	LSpriteInfo ExampleInfo;
	
	ExampleInfo.iNumRow = 1;
	ExampleInfo.iNumColumn = 3;
	ExampleInfo.fAnimTimer = 1.0f;
	ExampleInfo.fElapsedTimer = 0.0f;
	ExampleInfo.p = { 1.0f, 1.0f, 1.0f };
	ExampleInfo.s = { 1.0f, 1.0f, 1.0f };
	ExampleInfo.texList.clear();
	ExampleInfo.texFile = L"";
	ExampleInfo.texAlphaFile = L"";
	ExampleInfo.shaderFile = L"";

	LTexture* m_pAlphaTex = nullptr;
	float	m_fAnimTimer = 1.0f;  // 전체 시간
	float	m_fOffsetTime = 0.0f;	// 1프레임 교체시간
	float	m_fElapsedTimer = 0.0f; // 누적시간
	int		m_iCurrentAnimIndex = 0;
	int		m_iNumSpriteX = 1;
	int		m_iNumSpriteY = 1;

	return true;
}

bool Sample::Frame()
{
	LSpriteObj::GetInstance().Frame();
	LSpriteObj::Load(ExampleInfo)

	return true;
}

bool Sample::Render()
{
	LSpriteObj::GetInstance().Render();

	return true;
}

bool Sample::Release()
{
	LSpriteObj::GetInstance().Release();

	return true;
}

Sample::~Sample() {}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR IpCmdLine, int nCmdShow)
{
	Sample win;
	win.SetRegisterWindowClass(hInstance);
	win.SetCreateWindow(L"TeamBProject", 800, 600);
	win.Run();
}


