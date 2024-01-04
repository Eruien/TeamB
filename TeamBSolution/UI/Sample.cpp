#include "Sample.h"
#include "LGlobal.h"
#include "LPlaneObj.h"
#include "KSpriteObj.h"
#include "MoveObject.h"
#include "PickingUI.h"
#include "DragUI.h"
#include "Resize2D.h"
#include "imgui_test.h"
#include "ImGuiManager.h"

shared_ptr<KSpriteObj> Sample::_clickedObject = nullptr;

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
	_imGuiManager = make_shared< ImGuiManager>();
	_imgui = make_shared< imgui_test>();
	_imGuiManager->Init();
	_imgui->Init();

	

	sObj = make_shared<KSpriteObj>();
	//scripts
	sObj->AddScripts(make_shared<PickingUI>());
	sObj->AddScripts(make_shared<Resize2D>());
	sObj->AddScripts(make_shared<DragUI>());

	sObj->SetPos({ 0,0,0 });
	sObj->SetScale({ 200,200,1 });
	sObj->SetRect(sObj->m_vPosition, sObj->m_vScale);
	sObj->Init();
	sObj->SetName("Button1");
	sObj->Create(L"../../res/hlsl/CustomizeMap.hlsl", L"../../res/effect/damaged1.png");
	sObj->CreateAnimation(info);

	_objects.push_back(sObj);

	sObj2 = make_shared<KSpriteObj>();
	//scripts
	sObj2->AddScripts(make_shared<PickingUI>());
	sObj2->AddScripts(make_shared<Resize2D>());
	sObj2->AddScripts(make_shared<DragUI>());

	sObj2->SetPos({ 200,200,0 });
	sObj2->SetScale({ 200,200,1 });
	sObj2->SetRect(sObj->m_vPosition, sObj->m_vScale);
	sObj2->Init();
	sObj2->SetName("Button2");
	sObj2->Create(L"../../res/hlsl/CustomizeMap.hlsl", L"../../res/effect/damaged1.png");
	sObj2->CreateAnimation(info);
	
	_objects.push_back(sObj2);



//	obj = make_shared< LPlaneObj>();
//	obj->Set();
//	obj->Create(L"../../res/hlsl/CustomizeMap.hlsl", L"../../res/effect/damaged1.png");
//;	obj->Init();
//	obj->SetPos({ 1000,100,1 });
//	obj->SetScale({ 200,200,10 });

	return true;
}

bool Sample::Frame()
{

	// 버튼들의 isClicked 초기화

	_imGuiManager->Frame();
	_imgui->Frame();


	
		//for (auto obj : _objects)
		//{
		//

		//	if (static_pointer_cast<PickingUI>(obj->GetScript(L"PickingUI"))->GetIsSelected())
		//	{

		//		ImGui::Begin("Object Details");
		//		if (ImGui::InputText("##ObjectName", const_cast<char*>(obj->GetName().c_str()), sizeof(obj->GetName())))


		//			ImGui::Text("Position");

		//		// DragFloat 함수를 사용하여 X, Y, Z 값을 드래그 가능한 값으로 선택
		//		 // 왼쪽 정렬
		//		ImGui::Text("X");
		//		ImGui::SameLine(0, -1);
		//		ImGui::SetNextItemWidth(150);
		//		ImGui::DragFloat("##X", &obj->m_vPosition.x);


		//		ImGui::Text("Y");
		//		ImGui::SameLine(0, -1);
		//		ImGui::SetNextItemWidth(150);
		//		ImGui::DragFloat("##Y", &obj->m_vPosition.y);

		//		ImGui::Text("Z");
		//		ImGui::SameLine(0, -1);
		//		ImGui::SetNextItemWidth(150);
		//		ImGui::DragFloat("##Z", &obj->m_vPosition.z);



		//		// 여기에 오브젝트 정보나 설정을 표시하는 ImGui 컨트롤들을 추가

		//		ImGui::End();
		//		//static_pointer_cast<PickingUI>(obj->GetScript(L"PickingUI"))->SetIsSelected(false);
		//	}
	 //   }






	sObj->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
	sObj->Frame();
	sObj2->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
	sObj2->Frame();

	/*obj->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
	obj->Frame();*/
	return true;
}

bool Sample::Render()
{
	sObj->Render();
	sObj2->Render();
	//obj->Render();
	_imgui->Render();
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


