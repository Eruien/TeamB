#pragma once
#include "LStd.h"
#include "KObject.h"
//
#include "DigitDisplay.h"
#include "PickingUI.h"
#include "DragUI.h"
#include "Resize2D.h"
#include "Animator.h"
#include "ChangeTexture.h"
#include "ExitWindow.h"
#include "TextureList.h"
#include "BillBoard.h"
#include "ButtonAction.h"
#include "Text.h"
#include "SceneChange.h"
#include "HpBar.h"
#include "UIEvent.h"
//
#include "imgui_menuBar.h"
#include "ImGuiManager.h"
#include "Imgui_ObjectDetail.h"

class UIManager
{
public:
    static UIManager& GetInstance()
    {
        static UIManager instance;
        return instance;
    }

    // �����̺� ������



    void Init(ComPtr<ID3D11DepthStencilState> Depth, ComPtr<ID3D11DepthStencilState> Disable);
    void Frame();
    void Render();

    void Save(const wstring filePath);
    void Load(const wstring filePath);

    void AddUIObject(shared_ptr<KObject> obj) { _objs.push_back(obj); };
    shared_ptr<KObject> GetUIObject(wstring name);
    vector<shared_ptr<KObject>> GetGroup(wstring groupName);
    void RemoveObject(wstring name);
    	static bool s_isMouseInImGuiWindow;
        static shared_ptr<KObject> s_selectedObject;
        bool _debugMode = true;

private:
    vector<shared_ptr<KObject>> _objs;
    vector<shared_ptr<KObject>> _objsTemp;
    UIManager();
    ~UIManager();
    shared_ptr< Imgui_ObjectDetail> _imGuiObjDetail;
    shared_ptr< imgui_menuBar> _imgui_menuBar;
    shared_ptr<ImGuiManager> _imGuiManager;
    ComPtr<ID3D11DepthStencilState> _DepthStencilState;
    ComPtr<ID3D11DepthStencilState> _DepthStencilStateDisable;

};

