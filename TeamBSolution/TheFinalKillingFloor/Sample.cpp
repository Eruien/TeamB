#include "Sample.h"
#include "LGlobal.h"
#include "LInput.h"

bool Sample::Init()
{
	m_Scene = new LScene;
	m_Scene->FSM(FSMType::SCENE);

	return true;
}

bool Sample::Frame()
{
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


