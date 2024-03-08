#include "LScene.h"
#include "LGlobal.h"
#include "LMainScene.h"
#include "InGameScene.h"
#include "EndScene.h"
#include "ShopScene.h"
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
    m_pActionList.insert(std::make_pair(State::INGAMESCENE, std::make_unique<InGameScene>(this)));
    m_pActionList.insert(std::make_pair(State::ENDSCENE, std::make_unique<EndScene>(this)));
    m_pActionList.insert(std::make_pair(State::SHOPSCENE, std::make_unique<ShopScene>(this)));
    //
    m_pAction = m_pActionList.find(State::MAINSCENE)->second.get();
    m_CurrentState = State::MAINSCENE;
    
}

void LScene::SetTransition(Event inputEvent)
{
    m_CurrentState = m_pFsm->StateTransition(m_CurrentState, inputEvent);

    auto actionIter = m_pActionList.find(m_CurrentState);

    if (actionIter != m_pActionList.end()) {
        // Ű�� ������ ���
        m_pAction = actionIter->second.get();
    }
    else {
        // Ű�� �������� ���� ���
        std::cerr << "����: ���� ���¿� ���� �׼��� ã�� �� �����ϴ�." << std::endl;
        // �Ǵ� �⺻ �׼��� �����ϰų� �ٸ� ���� ó���� ������ �� �ֽ��ϴ�.
        //m_pAction = nullptr; // ������ nullptr�� �����߽��ϴ�.
    }
}

State LScene::GetState()
{
    return m_CurrentState;
}

void LScene::Process()
{
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


