#include "LScene.h"
#include "LGlobal.h"
#include "LMainScene.h"
#include "InGameScene.h"
#include "EndScene.h"
#include "ShopScene.h"
#include "SelectScene.h"
#include "SwordShopScene.h"
#include "UIManager.h"
using std::thread;

void LScene::InitThread()
{
    m_pActionList.insert(std::make_pair(State::INGAMESCENE, std::make_unique<InGameScene>(this)));

    m_pActionList.insert(std::make_pair(State::ENDSCENE, std::make_unique<EndScene>(this)));
    m_pActionList.insert(std::make_pair(State::SHOPSCENE, std::make_unique<ShopScene>(this)));
    m_pActionList.insert(std::make_pair(State::SWORDSHOPSCENE, std::make_unique<SwordShopScene>(this)));
    m_pActionList.insert(std::make_pair(State::SELECTSCENE, std::make_unique<SelectScene>(this)));

    LGlobal::g_IsLoding = false;
}

void LScene::FSM(FSMType fsmType)
{
    auto iter = LFSMMgr::GetInstance().m_map.find(fsmType);

    if (LFSMMgr::GetInstance().m_map.end() == iter)
    {
        MessageBoxA(NULL, "FSMType Error", "Error Box", MB_OK);
        return;
    }

    m_pFsm = iter->second.get();

    m_pActionList.insert(std::make_pair(State::MAINSCENE, std::make_unique<LMainScene>(this)));
    m_pActionList.insert(std::make_pair(State::LOADSCENE, std::make_unique<LMainScene>(this)));
    // loading scene 추가

    thread t1(&LScene::InitThread, this);

    
    
    //
    m_pAction = m_pActionList.find(State::LOADSCENE)->second.get();
    m_CurrentState = State::LOADSCENE;
    // 로딩신으로 대체
    
    t1.detach();
}

void LScene::SetTransition(Event inputEvent)
{
    m_CurrentState = m_pFsm->StateTransition(m_CurrentState, inputEvent);

    auto actionIter = m_pActionList.find(m_CurrentState);

    if (actionIter != m_pActionList.end()) {
        // 키가 존재할 경우
        m_pAction = actionIter->second.get();
    }
    else {
        // 키가 존재하지 않을 경우
        std::cerr << "에러: 현재 상태에 대한 액션을 찾을 수 없습니다." << std::endl;
        // 또는 기본 액션을 설정하거나 다른 오류 처리를 수행할 수 있습니다.
        //m_pAction = nullptr; // 예제로 nullptr을 설정했습니다.
    }
}

State LScene::GetState()
{
    return m_CurrentState;
}

void LScene::Process()
{
    if (LGlobal::g_IsLoding == false)
    {
        // state 변경
        UIManager::GetInstance().ChangeScene(Event::GOMAINSCENE);
        LGlobal::g_IsLoding = true;
    }
    m_pAction->Process();
}

void LScene::Render()
{
    m_pAction->Render();
}

bool LScene::Init()
{
    return true;
}

bool LScene::Frame()
{
    return true;
}

bool LScene::Release()
{
    return true;
}



LScene::LScene() {}
LScene::~LScene() {}


