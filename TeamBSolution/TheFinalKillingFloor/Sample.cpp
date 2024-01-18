#include "Sample.h"
#include "LGlobal.h"

bool Sample::Init()
{
	m_DebugCamera = std::make_shared<LDebugCamera>();
	m_DebugCamera->CreateLookAt({ 0.0f, 200.0f, -100.0f }, { 0.0f, 0.0f, 1.0f });
	m_DebugCamera->CreatePerspectiveFov(L_PI * 0.25, (float)LGlobal::g_WindowWidth / (float)LGlobal::g_WindowHeight, 1.0f, 10000.0f);
	LGlobal::g_pMainCamera = m_DebugCamera.get();

	m_ModelCamera = std::make_shared<LModelCamera>();
	m_ModelCamera->SetTargetPos(TVector3(0.0f, 0.0f, 0.0f));
	m_ModelCamera->CreatePerspectiveFov(L_PI * 0.25, (float)LGlobal::g_WindowWidth / (float)LGlobal::g_WindowHeight, 1.0f, 10000.0f);
	m_ModelCamera->m_fRadius = 100.0f;

	m_Scene = new LScene;
	m_Scene->FSM(FSMType::SCENE);

	return true;
}

bool Sample::Frame()
{
	/*if (LInput::GetInstance().m_dwKeyState['1'] == DWORD(KeyState::KEY_PUSH))
	{
		LGlobal::g_pMainCamera = m_DebugCamera.get();
	}

	if (LInput::GetInstance().m_dwKeyState['2'] == DWORD(KeyState::KEY_PUSH))
	{
		LGlobal::g_pMainCamera = m_ModelCamera.get();
	}*/

	m_Scene->Process();

	return true;
}

bool Sample::Render()
{
	m_Scene->Render();

	return true;
}

bool Sample::Release()
{
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


