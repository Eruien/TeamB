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
    // 델리게이트 타입 정의
    using Delegate = std::function<void()>;

    // 델리게이트 추가
    
    void AddDelegate(const std::string& key, Delegate delegate)
    {
        _delegateMap[key] = delegate;
    }

    // 델리게이트 실행
    void ExecuteDelegate(const std::string& key)
    {
        auto it = _delegateMap.find(key);
        if (it != _delegateMap.end())
        {
            // 델리게이트 실행
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