#include "Sample.h"
#include "LGlobal.h"
#include "LInput.h"

bool Sample::Init()
{

	m_UICamera = std::make_shared<UICamera>();
	m_UICamera->CreateLookAt({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
	m_UICamera->m_fCameraPitch = 0.0f;
	m_UICamera->CreateOrthographic((float)LGlobal::g_WindowWidth, (float)LGlobal::g_WindowHeight, 0, 1);
	LGlobal::g_pUICamera = m_UICamera.get();

	UIManager::GetInstance().Init(m_pDepthStencilState,m_pDepthStencilStateDisable);


	m_Scene = new LScene;
	m_Scene->FSM(FSMType::SCENE);

	return true;
}

bool Sample::Frame()
{
	m_Scene->Process();
	UIManager::GetInstance().Frame();
	return true;
}

bool Sample::Render()
{
	m_Scene->Render();
	UIManager::GetInstance().Render();
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
	win.SetCreateWindow(L"TeamBProject", LGlobal::g_WindowWidth, LGlobal::g_WindowHeight);
	win.Run();
}


