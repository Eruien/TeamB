#include "Sample.h"
#include "LGlobal.h"

bool Sample::Init()
{

 //   LMapDesc info = {
	//	129, 129, 1,1,
	//	L"../../res/topdownmap.jpg",
	//	L"../../res/hlsl/CustomizeMap.hlsl"
	//};

    m_pMapObj = std::make_unique<LPlaneObj>();
	m_pMapObj.get()->Load({ 0.0f,0.0f ,0.0f }, {(float)LGlobal::g_WindowWidth, (float)LGlobal::g_WindowHeight, 1.0f},
		 L"../../res/effect/topdownmap.jpg",  L"../../res/hlsl/CustomizeMap.hlsl");
	//m_pMapObj = std::make_unique<LMap>();
	//m_pMapObj->Set();
	//m_pMapObj->SetPos({ 0.0f,0.0f ,0.0f });
	//m_pMapObj->SetScale(TVector3((float)LGlobal::g_WindowWidth, (float)LGlobal::g_WindowHeight, 1.0f));

	//m_pMapObj->Create(L"../../res/hlsl/CustomizeMap.hlsl", L"../../res/topdownmap.jpg");


	m_DebugCamera = std::make_shared<LDebugCamera>();

	//m_DebugCamera->CreateLookAt({ 0.0f, 200.0f, -100.0f }, { 0.0f, 0.0f, 1.0f });
	m_DebugCamera->CreateLookAt({ 0.0f, 0.0f, -1000.0f }, { 0.0f, 0.0f, 0.0f });//카메라 위치조정

	m_DebugCamera->CreatePerspectiveFov(L_PI * 0.25, (float)LGlobal::g_WindowWidth / (float)LGlobal::g_WindowHeight, 1.0f, 10000.0f);
	LGlobal::g_pMainCamera = m_DebugCamera.get();






	return true;
}

bool Sample::Frame()
{
	m_pMapObj->Frame();



	return true;
}

bool Sample::Render()
{

    //m_pImmediateContext->OMSetBlendState(m_AlphaBlend, 0, -1);//임시



    //m_pMapObj->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matOrthoProjection);//직교
	m_pMapObj->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);//원근

    m_pMapObj->Render();

	return true;
}

bool Sample::Release()
{
    if (m_pMapObj)m_pMapObj->Release();

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


