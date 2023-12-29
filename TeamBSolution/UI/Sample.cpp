#include "Sample.h"
#include "LGlobal.h"
#include "LPlaneObj.h"
#include "KSpriteObj.h"

bool Sample::Init()
{
	m_DebugCamera = std::make_shared<UICamera>();
	m_DebugCamera->CreateLookAt({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
	m_DebugCamera->m_fCameraPitch = 0.0f;
	m_DebugCamera->CreateOrthographic((float)LGlobal::g_WindowWidth, (float)LGlobal::g_WindowHeight, 1, 100);
	LGlobal::g_pMainCamera = m_DebugCamera.get();


	animInfo info;
	info.isLoop = true;
	info.name = L"Anim";
	info.keyFrames.push_back({ L"../../res/effect/inhaleeffect12.png",0.05f });
	info.keyFrames.push_back({ L"../../res/effect/star3.png",0.05f });


	sObj = make_shared<KSpriteObj>();
	sObj->SetPos({ 0,0,0 });
	sObj->SetScale({ 1,1,1 });
	sObj->Init();
	sObj->Create(L"../../res/hlsl/CustomizeMap.hlsl", L"../../res/effect/damaged1.png");
	sObj->CreateAnimation(info);
	




//	obj = make_shared< LPlaneObj>();
//	obj->Set();
//	obj->Create(L"../../res/hlsl/CustomizeMap.hlsl", L"../../res/effect/damaged1.png");
//;	obj->Init();
//	obj->SetPos({ 0,0,10 });
//	obj->SetScale({ 1000,1000,10 });

	return true;
}

bool Sample::Frame()
{
	sObj->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
	sObj->Frame();
	return true;
}

bool Sample::Render()
{
	
	sObj->Render();
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


