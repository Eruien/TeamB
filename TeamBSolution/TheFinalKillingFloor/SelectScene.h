#pragma once
#include "Light.h"
#include "LScene.h"
#include "LDebugCamera.h"
#include "LHeightMap.h"
#include "LModel.h"
#include "LWeaponMgr.h"
#include "LSelect.h"

class SelectScene :public SceneState
{
public:
	std::shared_ptr<LDebugCamera> m_DebugCamera = nullptr;
	std::shared_ptr<LHeightMap> m_CustomMap = nullptr;
	std::shared_ptr<LSkinningModel> m_GunMan = nullptr;
	std::shared_ptr<LSkinningModel> m_SwordMan = nullptr;
	std::shared_ptr<LWeapon> m_Rifle = nullptr;
	LSelect m_Select;
public:
	// light
	LIGHT_CONSTANT_BUFFER1	m_cbLight1;
	LIGHT_CONSTANT_BUFFER2	m_cbLight2;
	ComPtr<ID3D11Buffer>	m_pConstantBufferLight[2];
	Light					m_PointLight[1];
	float fLightStart = 1.0f;
	float fLightEnd = 0.1f;
public:
	bool Init() override;
	void Render() override;
	void Process() override;
	void Release();
public:
	void InitializeDebugCamera();
	void InitializeMap();
	void InitializeModel();
	void InitializeWeapon();
	void UpdateWeaponPosition();
	void InitializeLighting();
	ID3D11Buffer* CreateConstantBuffer(ID3D11Device* pd3dDevice, void* data, UINT iNumIndex, UINT iSize, bool bDynamic = false);
public:
	SelectScene(LScene* parent);
	virtual ~SelectScene();
};


