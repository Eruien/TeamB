#include "Sample.h"
#include "LGlobal.h"
#include "LSpriteObj.h"

bool Sample::Init()
{
	m_DebugCamera = std::make_shared<LDebugCamera>();
	m_DebugCamera->CreateLookAt({ 0.0f, 200.0f, -100.0f }, { 0.0f, 0.0f, 1.0f });
	m_DebugCamera->CreatePerspectiveFov(L_PI * 0.25, (float)LGlobal::g_WindowWidth / (float)LGlobal::g_WindowHeight, 1.0f, 10000.0f);
	LGlobal::g_pMainCamera = m_DebugCamera.get();
	//ExampleInfo.shaderFile = L"../../res/hlsl/PVShader.hlsl";
	//ExampleInfo.shaderFile = L"../../res/hlsl/CustomizeMap.hlsl";

    m_pMapObj = std::make_unique<LPlaneObj>();
    m_pMapObj->Set();
    m_pMapObj->SetPos({ 0.0f,0.0f ,0.0f });
    m_pMapObj->SetScale(TVector3(4000, 4000, 1.0f));
    m_pMapObj->Create(L"../../res/hlsl/PVShader.hlsl",
        L"../../res/ui1/0.png");

    m_pSpriteTexObj = std::make_unique<LSpriteTexture>();
    LSpriteInfo info;
    info.p = { 0.0f,0.0f, 0.0f };
    info.s = { 100.0f,100.0f,1.0f };
    info.fAnimTimer = 10.0f;
    info.texFile = L"../../res/ui1/0.png";
    info.texList.push_back(L"../../res/ui1/0.png");
    info.texList.push_back(L"../../res/ui1/1.png");
    info.texList.push_back(L"../../res/ui1/2.png");
    info.texList.push_back(L"../../res/ui1/3.png");
    info.texList.push_back(L"../../res/ui1/4.png");
    info.texList.push_back(L"../../res/ui1/5.png");
    info.texList.push_back(L"../../res/ui1/6.png");
    info.texList.push_back(L"../../res/ui1/7.png");
    info.texList.push_back(L"../../res/ui1/8.png");
    info.texList.push_back(L"../../res/ui1/9.png");

    info.shaderFile = L"../../res/hlsl/PVShader.hlsl";
    m_pSpriteTexObj->Load(info);
    m_SpriteList.insert(std::make_pair(m_iSpriteIndex++, m_pSpriteTexObj.get()));

    info.Reset();
    m_pSpriteUVObj = std::make_unique<LSpriteUV>();
    ZeroMemory(&info, sizeof(info));
    info.iNumRow = 4;

    info.iNumColumn = 4;
    info.fAnimTimer = 1.0f;
    info.p = { 200.0f,0.0f, 0.0f };
    info.s = { 100.0f,100.0f,1.0f };
    info.texFile = L"../../res/effect/CanonBomb-01.tga";
    info.shaderFile = L"../../res/hlsl/PVShader.hlsl";
    m_pSpriteUVObj->Load(info);
    m_SpriteList.insert(std::make_pair(m_iSpriteIndex++, m_pSpriteUVObj.get()));



	return true;
}

bool Sample::Frame()
{


    if (LInput::GetInstance().m_dwKeyState[VK_LBUTTON] == 2)
    {
        TVector3 mouse = LInput::GetInstance().GetWorldPos( LGlobal::g_WindowWidth , LGlobal::g_WindowHeight ,
            LGlobal::g_pMainCamera->m_vCameraPos.x, LGlobal::g_pMainCamera->m_vCameraPos.y);

        LParticle info;
        ZeroMemory(&info, sizeof(info));
        info.m_bLife = true;
        info.m_iID = rand() % m_SpriteList.size();
        info.m_vPos = mouse;
        info.m_vSclae = { 50.0f,50.0f,50.0f };
        info.m_iMaxFrame = m_SpriteList[info.m_iID]->GetMaxSize();
        info.m_fOffsetTime = m_SpriteList[info.m_iID]->m_fAnimTimer / info.m_iMaxFrame;
        m_ParticleList.push_back(info);
    }
    for (std::list<LParticle>::iterator iter = m_ParticleList.begin();
        iter != m_ParticleList.end();)
    {
        LParticle& info = *iter;
        info.Frame();
        if (info.m_bLife == false)
        {
            iter = m_ParticleList.erase(iter);
        }
        else
        {
            iter++;
        }

    }


	return true;
}

bool Sample::Render()
{
    m_pMapObj->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matOrthoProjection);
    m_pMapObj->Render();

    for (std::list<LParticle>::iterator iter = m_ParticleList.begin();
        iter != m_ParticleList.end(); iter++)
    {
        LParticle& info = *iter;
        LSpriteObj* pBaseObj = m_SpriteList[info.m_iID];
        pBaseObj->SetPos(info.m_vPos);
        pBaseObj->SetScale(info.m_vSclae);
        pBaseObj->Frame();
        pBaseObj->m_iCurrentAnimIndex = info.m_iCurrentAnimIndex;
        pBaseObj->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matOrthoProjection);
        pBaseObj->Render();
    }

	return true;
}

bool Sample::Release()
{
    if (m_pMapObj)m_pMapObj->Release();
    if (m_pSpriteUVObj) m_pSpriteUVObj->Release();
    if (m_pSpriteTexObj)m_pSpriteTexObj->Release();
    if (m_pSpriteAirObj) m_pSpriteAirObj->Release();

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


