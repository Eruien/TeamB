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
   
    
    // FunctionManager�� �������� ����
    void SetFunctionManager(shared_ptr<FunctionManager> functionManager)
    {
        this->functionManager = functionManager;
    }

    // FunctionManager�� �Լ� �߰�
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

    // FunctionManager���� �Լ� ����
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

    // �����̺� ������
    UIManager() : functionManager(nullptr) {}
    ~UIManager() {}
};

