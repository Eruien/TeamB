#include "LEffect.h"
bool LEffect::Init()
{
	//스프라이트
	//m_pSpriteTexObj = std::make_unique<LSpriteTexture>();
	LSpriteInfo Spliteinfo;
	//Spliteinfo.p = { 0.0f,0.0f,0.0f };
	//Spliteinfo.s = { 50.0f,50.0f,1.0f };
	//Spliteinfo.fAnimTimer = 10.0f;
	//Spliteinfo.texFile = L"../../res/ui1/0.png";
	//Spliteinfo.texList.push_back(L"../../res/ui1/0.png");
	//Spliteinfo.texList.push_back(L"../../res/ui1/1.png");
	//Spliteinfo.texList.push_back(L"../../res/ui1/2.png");
	//Spliteinfo.texList.push_back(L"../../res/ui1/3.png");
	//Spliteinfo.texList.push_back(L"../../res/ui1/4.png");
	//Spliteinfo.texList.push_back(L"../../res/ui1/5.png");
	//Spliteinfo.texList.push_back(L"../../res/ui1/6.png");
	//Spliteinfo.texList.push_back(L"../../res/ui1/7.png");
	//Spliteinfo.texList.push_back(L"../../res/ui1/8.png");
	//Spliteinfo.texList.push_back(L"../../res/ui1/9.png");

	//Spliteinfo.shaderFile = L"../../res/hlsl/CustomizeMap.hlsl";

	//Spliteinfo.bBilboard = false;
	//Spliteinfo.bFadeout = false;
	//Spliteinfo.bScreenEffect = false;


	//m_pSpriteTexObj->Load(Spliteinfo);
	//m_SpriteList.insert(std::make_pair(m_iSpriteIndex++, m_pSpriteTexObj.get()));
	Spliteinfo.Reset();

	//UV스프라이트
	// 
	//0인덱스 총구화염1
	m_pSpriteUVObj = std::make_unique<LSpriteUV>();
	ZeroMemory(&Spliteinfo, sizeof(Spliteinfo));
	Spliteinfo.iNumRow = 1;
	Spliteinfo.iNumColumn = 4;
	Spliteinfo.fAnimTimer = 5.0f;
	Spliteinfo.p = { 0.0f,0.0f,0.0f };
	Spliteinfo.s = { 20.0f,20.0f,1.0f };
	Spliteinfo.texFile = L"../../res/effect/shotone1.png";
	Spliteinfo.shaderFile = L"../../res/hlsl/CustomizeMap.hlsl";
	
	Spliteinfo.bBilboard = true;
	Spliteinfo.bFadeout = false;
	Spliteinfo.bScreenEffect = false;


	m_pSpriteUVObj->Load(Spliteinfo);
	m_SpriteList.insert(std::make_pair(m_iSpriteIndex++, m_pSpriteUVObj.get()));
	Spliteinfo.Reset();

	//1인덱스 총구화염2

	Spliteinfo.iNumRow = 1;
	Spliteinfo.iNumColumn = 4;
	Spliteinfo.fAnimTimer = 5.0f;
	Spliteinfo.p = { 0.0f,0.0f,0.0f };
	Spliteinfo.s = { 20.0f,20.0f,1.0f };
	Spliteinfo.texFile = L"../../res/effect/shottwo1.png";
	Spliteinfo.shaderFile = L"../../res/hlsl/CustomizeMap.hlsl";

	Spliteinfo.bBilboard = true;
	Spliteinfo.bFadeout = false;
	Spliteinfo.bScreenEffect = false;


	m_pSpriteUVObj->Load(Spliteinfo);
	m_SpriteList.insert(std::make_pair(m_iSpriteIndex++, m_pSpriteUVObj.get()));
	Spliteinfo.Reset();

	//2인덱스 총구화염3
	Spliteinfo.iNumRow = 4;
	Spliteinfo.iNumColumn = 1;
	Spliteinfo.fAnimTimer = 5.0f;
	Spliteinfo.p = { 0.0f,0.0f,0.0f };
	Spliteinfo.s = { 20.0f,20.0f,1.0f };
	Spliteinfo.texFile = L"../../res/effect/shotthree1.png";
	Spliteinfo.shaderFile = L"../../res/hlsl/CustomizeMap.hlsl";

	Spliteinfo.bBilboard = true;
	Spliteinfo.bFadeout = false;
	Spliteinfo.bScreenEffect = false;


	m_pSpriteUVObj->Load(Spliteinfo);
	m_SpriteList.insert(std::make_pair(m_iSpriteIndex++, m_pSpriteUVObj.get()));
	Spliteinfo.Reset();


	//스크린 텍스쳐이펙트
	//인덱스3 근접피격 발톱자국
	m_pScreenTexObj = std::make_unique<LSpriteTexture>();
	Spliteinfo.p = { 0.0f,0.0f,0.0f };
	//Spliteinfo.s = { (float) LGlobal::g_WindowWidth,(float) LGlobal::g_WindowHeight,1.0f };
	Spliteinfo.s = { 400.0f,300.0f,1.0f };

	Spliteinfo.fAnimTimer = 3.0f;
	Spliteinfo.texFile = L"../../res/effect/Craw.png";
	Spliteinfo.texList.push_back(L"../../res/effect/Craw.png");
	Spliteinfo.texList.push_back(L"../../res/effect/Craw.png");
	Spliteinfo.shaderFile = L"../../res/hlsl/CustomizeMap.hlsl";
	
	Spliteinfo.bBilboard = true;
	Spliteinfo.bFadeout = true;
	Spliteinfo.bScreenEffect = true;

	
	m_pScreenTexObj->Load(Spliteinfo);
	m_SpriteList.insert(std::make_pair(m_iSpriteIndex++, m_pScreenTexObj.get()));
	Spliteinfo.Reset();


	//인덱스4 명중시 피튀김
	//m_pSpriteUVObj = std::make_unique<LSpriteUV>();
	//ZeroMemory(&Spliteinfo, sizeof(Spliteinfo));
	Spliteinfo.iNumRow = 4;
	Spliteinfo.iNumColumn = 4;
	Spliteinfo.fAnimTimer = 5.0f;
	Spliteinfo.p = { 0.0f,0.0f,0.0f };
	Spliteinfo.s = { 10.0f,10.0f,1.0f };
	Spliteinfo.texFile = L"../../res/effect/bloodefect.png";
	Spliteinfo.shaderFile = L"../../res/hlsl/CustomizeMap.hlsl";

	Spliteinfo.bBilboard = true;
	Spliteinfo.bFadeout = false;
	Spliteinfo.bScreenEffect = false;


	m_pSpriteUVObj->Load(Spliteinfo);
	m_SpriteList.insert(std::make_pair(m_iSpriteIndex++, m_pSpriteUVObj.get()));
	Spliteinfo.Reset();


	return true;
}
bool LEffect::Frame()
{
	TVector3 screen = LGlobal::g_pMainCamera->m_vCameraPos + LGlobal::g_pMainCamera->m_vLook * 10;
	TVector3 mouse = LInput::GetInstance().GetWorldPos(LGlobal::g_WindowWidth, LGlobal::g_WindowHeight,
		LGlobal::g_pMainCamera->m_vCameraPos.x, LGlobal::g_pMainCamera->m_vCameraPos.y);


	//P 누른채로 마우스 좌클릭 마우스 위치 생성 인덱스0
	if (LInput::GetInstance().m_dwKeyState['P'] == DWORD(KeyState::KEY_HOLD))
	{


		if (LInput::GetInstance().m_dwKeyState[VK_LBUTTON] == DWORD(KeyState::KEY_PUSH))
		{

			LParticle Spliteinfo;
			ZeroMemory(&Spliteinfo, sizeof(Spliteinfo));
			Spliteinfo.m_bLife = true;
			Spliteinfo.m_iID = 0;

			Spliteinfo.m_bBilboard = m_SpriteList[Spliteinfo.m_iID]->m_bBilboard;
			Spliteinfo.m_bFadeout = m_SpriteList[Spliteinfo.m_iID]->m_bFadeout;
			Spliteinfo.m_bScreenEffect = m_SpriteList[Spliteinfo.m_iID]->m_bScreenEffect;



			if (Spliteinfo.m_bScreenEffect == true)
			{
				Spliteinfo.m_vPos = screen;
			}
			else
			{
				Spliteinfo.m_vPos = mouse;
				Spliteinfo.m_vPos.z = -10.0f;


			}

			Spliteinfo.m_vScale = m_SpriteList[Spliteinfo.m_iID]->m_vScale;
			Spliteinfo.m_iMaxFrame = /*m_SpriteList[Spliteinfo.m_iID]->*/4;//임시
			Spliteinfo.m_fOffsetTime = m_SpriteList[Spliteinfo.m_iID]->m_fAnimTimer / Spliteinfo.m_iMaxFrame;
			Spliteinfo.m_fAnimTimer = m_SpriteList[Spliteinfo.m_iID]->m_fAnimTimer;


			m_ParticleList.push_back(Spliteinfo);//생성된 이펙트


		}

	
		//P누른상태에서 Q클릭시 피격이펙트 인덱스3 생성
		if (LInput::GetInstance().m_dwKeyState['Q'] == DWORD(KeyState::KEY_PUSH))
		{
				LParticle Spliteinfo;
				ZeroMemory(&Spliteinfo, sizeof(Spliteinfo));
				Spliteinfo.m_bLife = true;
				Spliteinfo.m_iID = 3;

				Spliteinfo.m_bBilboard = m_SpriteList[Spliteinfo.m_iID]->m_bBilboard;
				Spliteinfo.m_bFadeout = m_SpriteList[Spliteinfo.m_iID]->m_bFadeout;
				Spliteinfo.m_bScreenEffect = m_SpriteList[Spliteinfo.m_iID]->m_bScreenEffect;



			if (Spliteinfo.m_bScreenEffect == true)
			{
				Spliteinfo.m_vPos = LGlobal::g_pMainCamera->m_vCameraPos + LGlobal::g_pMainCamera->m_vLook * 10;
			}
			else
			{
				Spliteinfo.m_vPos = {0.0f,0.0f,screen.z};

			}

				Spliteinfo.m_vScale = m_SpriteList[Spliteinfo.m_iID]->m_vScale;
				Spliteinfo.m_iMaxFrame = m_SpriteList[Spliteinfo.m_iID]->GetMaxSize();
				Spliteinfo.m_fOffsetTime = m_SpriteList[Spliteinfo.m_iID]->m_fAnimTimer / Spliteinfo.m_iMaxFrame;
				Spliteinfo.m_fAnimTimer = m_SpriteList[Spliteinfo.m_iID]->m_fAnimTimer;


				m_ParticleList.push_back(Spliteinfo);//생성된 이펙트
		}
		//P누른상태에서 W클릭시 피격 피튀김 이펙트 인덱스4 
		if (LInput::GetInstance().m_dwKeyState['W'] == DWORD(KeyState::KEY_PUSH))
			{
				LParticle Spliteinfo;
				ZeroMemory(&Spliteinfo, sizeof(Spliteinfo));
				Spliteinfo.m_bLife = true;
				Spliteinfo.m_iID = 4;

				Spliteinfo.m_bBilboard = m_SpriteList[Spliteinfo.m_iID]->m_bBilboard;
				Spliteinfo.m_bFadeout = m_SpriteList[Spliteinfo.m_iID]->m_bFadeout;
				Spliteinfo.m_bScreenEffect = m_SpriteList[Spliteinfo.m_iID]->m_bScreenEffect;


				Spliteinfo.m_vScale = m_SpriteList[Spliteinfo.m_iID]->m_vScale;
				Spliteinfo.m_iMaxFrame = m_SpriteList[Spliteinfo.m_iID]->GetMaxSize();
				Spliteinfo.m_fOffsetTime = m_SpriteList[Spliteinfo.m_iID]->m_fAnimTimer / Spliteinfo.m_iMaxFrame;
				Spliteinfo.m_fAnimTimer = m_SpriteList[Spliteinfo.m_iID]->m_fAnimTimer;

					if (Spliteinfo.m_bScreenEffect == true)
					{
						Spliteinfo.m_vPos = screen;


					}
					else
					{
						Spliteinfo.m_vPos = mouse;
						Spliteinfo.m_vPos.z = -10.0f;


					}




					m_ParticleList.push_back(Spliteinfo);//생성된 이펙트

				

			}
	}
	


	for (std::list<LParticle>::iterator iter = m_ParticleList.begin();
		iter != m_ParticleList.end();)
	{
		LParticle& Spliteinfo = *iter;
		Spliteinfo.Frame();
		if (Spliteinfo.m_bScreenEffect == true)
		{
			Spliteinfo.m_vPos = screen;
		}

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
bool LEffect::Render()
{
	m_iParticleIndex = 0;
	for (std::list<LParticle>::iterator iter = m_ParticleList.begin();
		iter != m_ParticleList.end(); iter++)
	{

		LParticle& Spliteinfo = *iter;
		LSpriteObj* pBaseObj = m_SpriteList[Spliteinfo.m_iID];
		pBaseObj->SetPos(m_SpriteList[Spliteinfo.m_iID]->m_vPosition);
		pBaseObj->SetScale(m_SpriteList[Spliteinfo.m_iID]->m_vScale);
		pBaseObj->Frame();
		pBaseObj->m_iCurrentAnimIndex = Spliteinfo.m_iCurrentAnimIndex;

			//if (pBaseObj->m_bBilboard == true)
			//{
			//	SetBilboardMatrix();

			//}
			pBaseObj->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);





		pBaseObj->Render();
#ifdef _DEBUG
		std::wstring text = L"DeburgCamera Pos (";
		
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

		text += L" Playtime: ";
		text += std::to_wstring((int)Spliteinfo.m_fPlayTime);
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
bool LEffect::Release()
{
	if (m_pSpriteUVObj) m_pSpriteUVObj->Release();
	//if (m_pSpriteTexObj)m_pSpriteTexObj->Release();
	if (m_pSpriteAirObj) m_pSpriteAirObj->Release();
	return true;
}
