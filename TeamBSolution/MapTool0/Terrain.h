#pragma once
#include "LCore.h"
#include "LStd.h"
#include "LQurdTree.h"
#include "LHeightMap.h"
#include "LSelect.h"
#include "CamForTool.h"
#include "TextureManager.h"
//#include <algorithm>

//
struct LIGHT_CONSTANT_BUFFER
{
	TMatrix g_matInvWorld;
	// material
	TVector4 g_cAmbientMaterial;	//
	TVector4 g_cDiffuseMaterial;	//
	TVector4 g_cSpecularMaterial;	//
	TVector4 g_cEmissionMaterial;	//
	// light
	TVector4 g_cAmbientLightColor;
	TVector4 g_cDiffuseLightColor;
	TVector4 g_cSpecularLightColor;
	TVector4 g_vLightDir; // w : light damping(attenuation) °¨¼è
	TVector4 g_vLightPos; // w : light radius
	TVector4 g_vEyeDir; // w : light intensity °­µµ
	TVector4 g_vEyePos; // w : light radius

};

class Terrain : public LCore
{
public:
	shared_ptr<CamForTool> m_DebugCamera = nullptr;
	LHeightMap* m_HeightMap = nullptr;
	LQurdtree* m_Tree = nullptr;
	shared_ptr<LSelect> m_Select = nullptr;

	//light
	TVector3 m_vLightVector;
	LIGHT_CONSTANT_BUFFER m_cbLight;
	ComPtr<ID3D11Buffer> m_pConstantBufferLight;

	UINT m_iSplattingTexIndex = 0;

	// for splatting
	ComPtr<ID3D10ShaderResourceView> m_pTexSRV[4];
	ComPtr<ID3D11PixelShader> m_pPS;

public:

	bool m_bObjectPicking = false;
	bool m_bUpPicking = false;
	bool m_bDownPicking = false;
	bool m_bPlatPicking = false;
	bool m_bSplatting = true;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

	ID3D11Buffer* CreateConstantBuffer(ID3D11Device* pd3dDevice, void* data, UINT iNumIndex,
					UINT iSize, bool bDynamic = false);
	bool GetIntersection();

	// for splating
	void TillingTexture();
	template <typename T>
	T MyClamp(const T& value, const T& low, const T& high) {
		return (value < low) ? low : (value > high) ? high : value;
	}
public:
	virtual ~Terrain();
};

