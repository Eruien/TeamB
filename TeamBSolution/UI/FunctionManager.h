#pragma once
#include <iostream>
#include <map>
#include <functional>
//struct args
//{
//    int  num1;
//    int  num2;
//    float num3;
//    float num4;
//};

class FunctionManager
{


public:
    // ��������Ʈ Ÿ�� ����
    using Delegate = std::function<void()>;

    // ��������Ʈ �߰�
    
    void AddDelegate(const std::string& key, Delegate delegate)
    {
        _delegateMap[key] = delegate;
    }

    // ��������Ʈ ����
    void ExecuteDelegate(const std::string& key)
    {
        auto it = _delegateMap.find(key);
        if (it != _delegateMap.end())
        {
            // ��������Ʈ ����
            it->second();
        }
        else
        {
            std::cout << "Delegate not found: " << key << std::endl;
        }
    }

public:
    void TestFunc();
    void Init();
    std::map<std::string, Delegate> GetMap() { return _delegateMap;};

private:
    std::map<std::string, Delegate> _delegateMap ;
};