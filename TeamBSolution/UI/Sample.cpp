#include "Sample.h"
#include "LGlobal.h"
#include "LPlaneObj.h"
#include "PickingUI.h"
#include "DragUI.h"
#include "Resize2D.h"
#include "imgui_menuBar.h"
#include "ImGuiManager.h"
#include "ImguiDetail.h"
#include "Animator.h"
#include "ChangeTexture.h"
#include "ExitWindow.h"
#include "UIManager.h"
#include "IntToSprite.h"
shared_ptr<KObject> Sample::s_selectedObject = nullptr;
bool Sample::s_isMouseInImGuiWindow = false;

bool Sample::Init()
{

	/*vector<wstring> num;
	num.push_back({ L"../../res/ui/0.png" });
	num.push_back({ L"../../res/ui/1.png" });
	num.push_back({ L"../../res/ui/2.png" });
	num.push_back({ L"../../res/ui/3.png" });
	num.push_back({ L"../../res/ui/4.png" });
	num.push_back({ L"../../res/ui/5.png" });
	num.push_back({ L"../../res/ui/6.png" });
	num.push_back({ L"../../res/ui/7.png" });
	num.push_back({ L"../../res/ui/8.png" });
	num.push_back({ L"../../res/ui/9.png" });*/

	m_DebugCamera = std::make_shared<UICamera>();
	m_DebugCamera->CreateLookAt({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
	m_DebugCamera->m_fCameraPitch = 0.0f;
	m_DebugCamera->CreateOrthographic((float)LGlobal::g_WindowWidth, (float)LGlobal::g_WindowHeight, 0, 1);
	LGlobal::g_pMainCamera = m_DebugCamera.get();


	animInfo info;
	info.isLoop = true;
	info.name = L"Anim";
	info.keyFrames.push_back({ L"../../res/effect/inhaleeffect12.png",1.f });
	info.keyFrames.push_back({ L"../../res/effect/star3.png",1.f });

	animInfo info2;
	info2.isLoop = true;
	info2.name = L"Anim2";
	info2.keyFrames.push_back({ L"../../res/ui/inhaleeffect12.png",1.f });
	info2.keyFrames.push_back({ L"../../res/ui/inhaleeffect12.png",1.f });
	info2.keyFrames.push_back({ L"../../res/ui/star3.png",1.f });
	info2.keyFrames.push_back({ L"../../res/ui/inhaleeffect12.png",1.f });


	_imGuiManager = make_shared< ImGuiManager>();
	
	_imGuiObjDetail = make_shared<Imgui_ObjectDetail>();
	_imgui_menuBar = make_shared<imgui_menuBar>();
	_imGuiManager->Init();
	_imGuiObjDetail->Init();
	_imgui_menuBar->Init();

	
	//obj1
	{
		sObj = make_shared<KObject>();
	//scripts
	sObj->AddScripts(make_shared<PickingUI>());
	sObj->AddScripts(make_shared<Resize2D>());
	sObj->AddScripts(make_shared<DragUI>());
	//sObj->AddScripts(make_shared<ImguiDetail>());
	sObj->AddScripts(make_shared<Animator>());
	sObj->AddScripts(make_shared<ChangeTexture>());
	sObj->SetPos({ 80,80,0 });
	sObj->SetScale({ 200,200,1 });
	sObj->SetRect(sObj->m_vPosition, sObj->m_vScale);
	sObj->Init();
	sObj->SetName("Button1");
	sObj->Create(L"../../res/hlsl/CustomizeMap.hlsl", L"../../res/effect/damaged1.png");
	sObj->GetScript<Animator>(L"Animator")->CreateAnimation(info);

	_objects.push_back(sObj);
	}
	//obj2
	{
		sObj2 = make_shared<KObject>();
		//scripts
		sObj2->AddScripts(make_shared<PickingUI>());
		sObj2->AddScripts(make_shared<Resize2D>());
		sObj2->AddScripts(make_shared<DragUI>());
		//sObj2->AddScripts(make_shared<ImguiDetail>());
		sObj2->AddScripts(make_shared<Animator>());
		sObj2->AddScripts(make_shared<ChangeTexture>());
		sObj2->SetPos({ 200,200,0 });
		sObj2->SetScale({ 200,200,1 });
		sObj2->SetRect(sObj->m_vPosition, sObj->m_vScale);
		sObj2->Init();
		sObj2->SetName("Button2");
		sObj2->Create(L"../../res/hlsl/CustomizeMap.hlsl", L"../../res/effect/damaged1.png");
		sObj2->GetScript<Animator>(L"Animator")->CreateAnimation(info2);

		_objects.push_back(sObj2);
	}
	//obj3
	{
		obj = make_shared< KObject>();
		obj->AddScripts(make_shared<PickingUI>());
		obj->AddScripts(make_shared<Resize2D>());
		obj->AddScripts(make_shared<DragUI>());
		//obj->AddScripts(make_shared<ImguiDetail>());
		obj->AddScripts(make_shared<ChangeTexture>());
		//obj->AddScripts(make_shared<ExitWindow>());
		obj->Set();
		obj->SetName("Button3");
		obj->Create(L"../../res/hlsl/CustomizeMap.hlsl", L"../../res/ui/8.png");
		
		obj->Init();
		obj->SetPos({ 100,100,1 });
		obj->SetScale({ 100,100,10 });
	}

	nObj = make_shared<NumberObject>(10);
	nObj->AddScripts(make_shared<PickingUI>());
	nObj->AddScripts(make_shared<Resize2D>());
	nObj->AddScripts(make_shared<DragUI>());
	nObj->SetName("Ammo");
	nObj->SetPos({ -100,100,1 });
	nObj->SetScale({ 400,100,10 });
	nObj->Init();
	nObj->Create(L"../../res/hlsl/CustomizeMap.hlsl", L"..\\..\\res\\effect\\damaged1.png");
	nObj->UpdateNumber();
	
	return true;
}

bool Sample::Frame()
{
	//UIManager::GetInstance().ExecuteDelegate("MemberFunctionDelegate");

	_imGuiManager->Frame();
	_imGuiObjDetail->Frame();
	_imgui_menuBar->Frame();


	

	sObj->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
	sObj->Frame();
	sObj2->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
	sObj2->Frame();

	obj->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
	obj->Frame();

	nObj->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
	nObj->Frame();
	nObj->UpdateNumber();
	return true;
}

bool Sample::Render()
{
	nObj->Render();
	sObj->Render();
	sObj2->Render();
	obj->Render();
	_imGuiObjDetail->Render();
	_imgui_menuBar->Render();
	_imGuiManager->Render();
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


