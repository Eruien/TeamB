#include "Sample.h"
#include "LGlobal.h"
#include "LSpriteObj.h"
#define randstep(fmin, fmax) ((float)fmin+((float)fmax-(float)fmin)* rand() / RAND_MAX)//추가

bool Sample::Init()
{

 //   LMapDesc info = {
	//	129, 129, 1,1,
	//	L"../../res/topdownmap.jpg",
	//	L"../../res/hlsl/CustomizeMap.hlsl"
	//};

    m_pMapObj = std::make_unique<LPlaneObj>();
	m_pMapObj.get()->Load({ 0.0f,0.0f ,0.0f }, {(float)LGlobal::g_WindowWidth, (float)LGlobal::g_WindowHeight, 1.0f},
		 L"../../res/topdownmap.jpg",  L"../../res/hlsl/CustomizeMap.hlsl");
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


	//스프라이트
	m_pSpriteTexObj = std::make_unique<LSpriteTexture>();
	LSpriteInfo Spliteinfo;
	Spliteinfo.p = { 0.0f,0.0f, 100.0f };
	Spliteinfo.s = { 100.0f,100.0f,1.0f };
	Spliteinfo.fAnimTimer = 10.0f;
	Spliteinfo.texFile = L"../../res/ui1/0.png";
	Spliteinfo.texList.push_back(L"../../res/ui1/0.png");
	Spliteinfo.texList.push_back(L"../../res/ui1/1.png");
	Spliteinfo.texList.push_back(L"../../res/ui1/2.png");
	Spliteinfo.texList.push_back(L"../../res/ui1/3.png");
	Spliteinfo.texList.push_back(L"../../res/ui1/4.png");
	Spliteinfo.texList.push_back(L"../../res/ui1/5.png");
	Spliteinfo.texList.push_back(L"../../res/ui1/6.png");
	Spliteinfo.texList.push_back(L"../../res/ui1/7.png");
	Spliteinfo.texList.push_back(L"../../res/ui1/8.png");
	Spliteinfo.texList.push_back(L"../../res/ui1/9.png");

	Spliteinfo.shaderFile = L"../../res/hlsl/CustomizeMap.hlsl";

	m_pSpriteTexObj->Load(Spliteinfo);
	m_SpriteList.insert(std::make_pair(m_iSpriteIndex++, m_pSpriteTexObj.get()));
	Spliteinfo.Reset();

	//UV스프라이트
	m_pSpriteUVObj = std::make_unique<LSpriteUV>();
	ZeroMemory(&Spliteinfo, sizeof(Spliteinfo));
	Spliteinfo.iNumRow = 4;

	Spliteinfo.iNumColumn = 4;
	Spliteinfo.fAnimTimer = 4.0f;
	Spliteinfo.p = { 200.0f,0.0f, 0.0f };
	Spliteinfo.s = { 100.0f,100.0f,1.0f };
	Spliteinfo.texFile = L"../../res/effect/CanonBomb-01.tga";
	Spliteinfo.shaderFile = L"../../res/hlsl/CustomizeMap.hlsl";

	m_pSpriteUVObj->Load(Spliteinfo);
	m_SpriteList.insert(std::make_pair(m_iSpriteIndex++, m_pSpriteUVObj.get()));
	Spliteinfo.Reset();




	return true;
}

bool Sample::Frame()
{
	m_pMapObj->Frame();

	if (LInput::GetInstance().m_dwKeyState['P'] == DWORD(KeyState::KEY_PUSH))
	{
		TVector3 mouse = LInput::GetInstance().GetWorldPos(LGlobal::g_WindowWidth, LGlobal::g_WindowHeight,
			LGlobal::g_pMainCamera->m_vCameraPos.x, LGlobal::g_pMainCamera->m_vCameraPos.y);

		LParticle Spliteinfo;
		ZeroMemory(&Spliteinfo, sizeof(Spliteinfo));
		Spliteinfo.m_bLife = true;
		Spliteinfo.m_iID = rand() % m_SpriteList.size();
		Spliteinfo.m_vPos = mouse;
		Spliteinfo.m_vPos.z = -900.0f;//z위치 랜덤생성 실사용시 입력받으면됨

		Spliteinfo.m_vSclae = { 50.0f,50.0f,50.0f };
		Spliteinfo.m_iMaxFrame = m_SpriteList[Spliteinfo.m_iID]->GetMaxSize();
		Spliteinfo.m_fOffsetTime = m_SpriteList[Spliteinfo.m_iID]->m_fAnimTimer / Spliteinfo.m_iMaxFrame;
		m_ParticleList.push_back(Spliteinfo);
	}

	for (std::list<LParticle>::iterator iter = m_ParticleList.begin();
		iter != m_ParticleList.end();)
	{
		LParticle& Spliteinfo = *iter;
		Spliteinfo.Frame();
		if (Spliteinfo.m_bLife == false)
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

    //m_pImmediateContext->OMSetBlendState(m_AlphaBlend, 0, -1);//임시



    //m_pMapObj->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matOrthoProjection);//직교
	m_pMapObj->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);//원근

    m_pMapObj->Render();
	m_iParticleIndex=0;
    for (std::list<LParticle>::iterator iter = m_ParticleList.begin();
        iter != m_ParticleList.end(); iter++)
    {
		
        LParticle& Spliteinfo = *iter;
        LSpriteObj* pBaseObj = m_SpriteList[Spliteinfo.m_iID];
        pBaseObj->SetPos(Spliteinfo.m_vPos);
        pBaseObj->SetScale(Spliteinfo.m_vSclae);
        pBaseObj->Frame();
        pBaseObj->m_iCurrentAnimIndex = Spliteinfo.m_iCurrentAnimIndex;
        pBaseObj->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
        pBaseObj->Render();
#ifdef _DEBUG
		std::wstring text;
		for (int i = 0; i < m_iParticleIndex; i++)
		{
			text += L"\n"; text += L")\n"; text += L")\n";

		}
		text += L"EffectIndex: ";
		text += std::to_wstring(Spliteinfo.m_iID);
		text += L"\n";
		
		text += L"EffectPosition (";
		text += std::to_wstring(Spliteinfo.m_vPos.x);
		text += L",";
		text += std::to_wstring(Spliteinfo.m_vPos.y);
		text += L",";
		text += std::to_wstring(Spliteinfo.m_vPos.z);
		text += L")\n";

		text += L" Remain time: ";
		text += std::to_wstring((int)Spliteinfo.m_fAnimTimer- (int)Spliteinfo.m_fPlayTime);
		text += L"s";

		

		text += L"  #Remain EffectNum :";
		text += std::to_wstring(m_ParticleList.size());
		text += L")\n";

	
		LWrite::GetInstance().AddText(text, 0.0f, 200.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
#endif
		m_iParticleIndex++;
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


