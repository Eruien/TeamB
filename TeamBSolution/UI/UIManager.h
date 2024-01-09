#pragma once
#include "FunctionManager.h"
#include "LStd.h"
class UIManager
{
public:
    static UIManager& GetInstance()
    {
        static UIManager instance;
        return instance;
    }
   
    
    // FunctionManager를 가지도록 수정
    void SetFunctionManager(shared_ptr<FunctionManager> functionManager)
    {
        this->functionManager = functionManager;
    }

    // FunctionManager에 함수 추가
    void AddDelegate(const std::string& identifier, FunctionManager::Delegate function)
    {
        if (functionManager)
        {
            functionManager->AddDelegate(identifier, function);
        }
        else
        {
            std::cout << "FunctionManager not set." << std::endl;
        }
    }

    // FunctionManager에서 함수 실행
    void ExecuteDelegate(const std::string& identifier)
    {
        if (functionManager)
        {
            functionManager->ExecuteDelegate(identifier);
        }
        else
        {
            std::cout << "FunctionManager not set." << std::endl;
        }
    }

    void Init();
    void Frame();
    void Render();



private:
    shared_ptr<FunctionManager> functionManager;

    // 프라이빗 생성자
    UIManager() : functionManager(nullptr) {}
    ~UIManager() {}
};

