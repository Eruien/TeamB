#include "Sample.h"
#include "LGlobal.h"
#include "LPlaneObj.h"
#include "KSpriteObj.h"
#include "MoveObject.h"
#include "Button.h"
bool Sample::Init()
{
	m_DebugCamera = std::make_shared<UICamera>();
	m_DebugCamera->CreateLookAt({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
	m_DebugCamera->m_fCameraPitch = 0.0f;
	m_DebugCamera->CreateOrthographic((float)LGlobal::g_WindowWidth, (float)LGlobal::g_WindowHeight, 0, 1);
	LGlobal::g_pMainCamera = m_DebugCamera.get();


	animInfo info;
	info.isLoop = true;
	info.name = L"Anim";
	//info.keyFrames.push_back({ L"../../res/effect/inhaleeffect12.png",0.05f });
	info.keyFrames.push_back({ L"../../res/effect/star3.png",0.05f });


	sObj = make_shared<KSpriteObj>();
	sObj->SetPos({ 0,0,0 });
	sObj->SetScale({ 200,200,1 });
	sObj->SetRect(sObj->m_vPosition, sObj->m_vScale);
	sObj->Init();
	sObj->Create(L"../../res/hlsl/CustomizeMap.hlsl", L"../../res/effect/damaged1.png");
	sObj->CreateAnimation(info);
	//sObj->AddScripts(make_shared<MoveObject>());
	sObj->AddScripts(make_shared<Button>());
	static_pointer_cast<Button>(sObj->GetScript(L"Button"));



	obj = make_shared< LPlaneObj>();
	obj->Set();
	obj->Create(L"../../res/hlsl/CustomizeMap.hlsl", L"../../res/effect/damaged1.png");
;	obj->Init();
	obj->SetPos({ 1000,100,1 });
	obj->SetScale({ 200,200,10 });

	return true;
}

bool Sample::Frame()
{
	
	sObj->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
	sObj->Frame();
	obj->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
	obj->Frame();
	return true;
}

bool Sample::Render()
{
	
	sObj->Render();
	obj->Render();
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
	win.SetCreateWindow(L"TeamBProject", (float)LGlobal::g_WindowWidth, (float)LGlobal::g_WindowHeight);
	win.Run();
}


