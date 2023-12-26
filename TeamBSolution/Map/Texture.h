#pragma once
#include "LStd.h"
#include "ResourceBase.h"
#include "LGlobal.h"
#include "Utils.h"
#include "LCore.h"

class Texture : public ResourceBase
{
private:
	using Base = ResourceBase;
	ComPtr<ID3D11ShaderResourceView> shaderResourceView;  // SRV
	TVector2 size;
	
public:
	Texture();
	virtual ~Texture();

	virtual bool Load(const wstring& path) override;

	ComPtr<ID3D11ShaderResourceView> GetShaderResourceView() { return shaderResourceView; }
	TVector2 GetSize() { return size; }
	const shared_ptr<::ScratchImage> GetInfo();
	
	void CreateTexture(int width, int height);
	void UpdateTexture(const vector<BYTE>& colors);
	void SaveTexture(const wstring& path);
	
	void GetTextureRGBAData(vector<BYTE>& colors);

};

