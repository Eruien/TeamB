#pragma once
#include "ResourceBase.h"
class TextureList : public ResourceBase
{
public:
	TextureList();
	~TextureList();
	void Save(wstring path);
	void Load(wstring path);
	void AddTexture(wstring path) { _texList.push_back(path); };
	void SetTexList(vector<wstring> paths) { _texList = paths; };

public :
	vector<wstring> _texList;
};

