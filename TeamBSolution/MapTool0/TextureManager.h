#pragma once
#include "Texture.h"
#include "atlconv.h"

typedef std::basic_string<wchar_t> W_STR;
template<class T> class Singleton
{
public:
	static T& GetInstance()
	{
		static T theSingleInstance;
		return theSingleInstance;
	}
};

class TextureManager : public Singleton<TextureManager>
{
public:
	ID3D11Device* m_pd3dDevice = nullptr;// 디바이스 객체
	ID3D11DeviceContext* m_pImmediateContext = nullptr;
	void		  SetDevice(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext);
private:
	friend class Singleton<TextureManager>;
	std::map<std::wstring, Texture*> m_List;
public:
	Texture* Load(std::wstring name);
	Texture* Find(std::wstring name);
	bool		Release();
	W_STR		GetSplitName(std::wstring fullpath);
	W_STR		GetSplitName(std::string fullpath);
private:
	TextureManager();
public:
	~TextureManager();
};

#define I_Tex TextureManager::GetInstance()
