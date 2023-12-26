#include "pch.h"
#include "Sample.h"
#include "LGlobal.h"

bool Sample::Init()
{
	TerrainDesc info;

	info.rowNum = 100;
	info.colNum = 100;
	info.cellDistance = 1;
	info.heightScale = 0.25f;
	info.shaderFilePath = L"../../res/hlsl/MapToolShader.fx";
	info.heightMapFilePath = L"../../res/Terrain/height513.bmp";
	info.alphaTexPath = L"../../Res/Textures/Terrain/heightExported.jpg";
	info.textureFilePath = L"../../Res/Textures/Terrain/grass.jpg";
	info.DevideTreeDepth = 2;

	map = make_shared<Terrain>(info);
	map->Init(info.rowNum, info.colNum, info.heightScale, info.);
	//map->Init();

	m_DebugCamera = std::make_shared<LDebugCamera>();
	m_DebugCamera->CreateLookAt({ 0.0f, 200.0f, -100.0f }, { 0.0f, 0.0f, 1.0f });
	m_DebugCamera->CreatePerspectiveFov(L_PI * 0.25, (float)LGlobal::g_WindowWidth / (float)LGlobal::g_WindowHeight, 1.0f, 10000.0f);
	LGlobal::g_pMainCamera = m_DebugCamera.get();

	// 텍스처랑 상관없음 맵 크기를 정하면 그 크기만큼 맵이 만들어진다.
	// 크기는 2의n승 +1 크기만 가능하다.
	// ex 513, 1025,

	return true;
}

bool Sample::Frame()
{
	m_HeightMap->Frame();

	return true;
}

bool Sample::Render()
{
	m_HeightMap->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
	m_HeightMap->Render();

	return true;
}

bool Sample::PreRender()
{
	return false;
}

bool Sample::PostFrame()
{
	return false;
}

bool Sample::Release()
{
	return true;
}

bool Sample::PostRender()
{
	return false;
}

Sample::~Sample() {}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR IpCmdLine, int nCmdShow)
{
	Sample win;
	win.SetRegisterWindowClass(hInstance);
	win.SetCreateWindow(L"TeamBProject", 800, 600);
	win.Run();
}


