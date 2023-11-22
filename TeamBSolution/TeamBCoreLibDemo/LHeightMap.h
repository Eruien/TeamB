#pragma once
#include "LMap.h"
#include <WICTextureLoader.h>
#pragma comment(lib, "DirectXTK_D.lib")

class LHeightMap : public LMap
{
public:
	std::vector<float> m_fHeightList;
public:
	ID3D11Resource* m_pTexture;
public:
	void CreateHeightMap(std::wstring heightMapFilePath);
	float GetHeightmap(int row, int col);
public:
	float GetHeightOfVertex(int Index) override;
	LVector4 GetColorOfVertex(int Index) override;
};

