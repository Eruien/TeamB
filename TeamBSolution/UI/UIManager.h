#pragma once
#include "LStd.h"
#include "KObject.h"
//
#include "IntToSprite.h"
#include "DigitDisplay.h"
#include "PickingUI.h"
#include "DragUI.h"
#include "Resize2D.h"
#include "Animator.h"
#include "ChangeTexture.h"
#include "ExitWindow.h"
#include "TextureList.h"
//

class UIManager
{
public:
    static UIManager& GetInstance()
    {
        static UIManager instance;
        return instance;
    }

    // 프라이빗 생성자

    
  
    void Init();
    void Frame();
    void Render();

    void Save(const wstring filePath);
    void Load(const wstring filePath);

    void AddUIObject(shared_ptr<KObject> obj) { _objs.push_back(obj); };
    shared_ptr<KObject> GetUIObject(wstring name);




private:
    vector<shared_ptr<KObject>> _objs;
    UIManager();
    ~UIManager();
   
};

