#pragma once
#include "MonoBehaviour.h"

class TextureList;
class ButtonAction : public MonoBehaviour
{
public:
	ButtonAction(wstring texPaths, wstring bFuntion);
	~ButtonAction();
public:
	void Frame() override;
	void LoadTextureList(wstring texXmlPath);

	wstring GetXmlPath() { return _texXmlPath; };
public:
	//버튼기능
	void MainSceneOption();
	void MainSceneOptionClose();
	void RetryButton();
	void SoundToggle();
	void GoInGameScene();
	void GoMainScene();
	void GoEndScene();

	std::unordered_map<wstring, void (ButtonAction::*)()> _functionMap;
	wstring _function = L"None";
private:
	wstring _texXmlPath;
	shared_ptr<TextureList> _texList;
	bool _toggleSound = true;
	
	
};

