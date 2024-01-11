#pragma once
#include "LStd.h"
#include "NumberObject.h"
class UIManager
{
public:
    static UIManager& GetInstance()
    {
        static UIManager instance;
        return instance;
    }
   
    
  
    void Init();
    void Frame();
    void Render();

    void Save();
    void Load();

    void AddUIObject(shared_ptr<KObject> obj) { _objs.push_back(obj); };
    shared_ptr<KObject> GetUIObject(wstring name);




private:
    vector<shared_ptr<KObject>> _objs;

    // 프라이빗 생성자
    UIManager();
    ~UIManager();
};

