#pragma once
#include "MonoBehaviour.h"

class TextureList;
class ButtonAction : public MonoBehaviour
{
public:
	ButtonAction(wstring texPaths);
	~ButtonAction();
public:
	void Frame() override;
	void LoadTextureList(wstring texXmlPath);

	wstring GetXmlPath() { return _texXmlPath; };
private:
	wstring _texXmlPath;

	shared_ptr<TextureList> _texList;
	
};

