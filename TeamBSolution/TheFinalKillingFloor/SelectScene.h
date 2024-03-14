#pragma once
#include "Light.h"
#include "LSkyBox.h"
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
	std::shared_ptr<LSkyBox> m_SkyBox = nullptr;
	std::shared_ptr<LHeightMap> m_CustomMap = nullptr;
	std::shared_ptr<LSkinningModel> m_GunMan = nullptr;
	std::shared_ptr<LSkinningModel> m_SwordMan = nullptr;
	std::shared_ptr<LWeapon> m_Rifle = nullptr;
	std::shared_ptr<LWeapon> m_OneHandSword = nullptr;
	LSelect m_Select;
public:
	// 처음 카메라 세팅
	TVector3 m_BindCameraPos = { -28.948f, 71.073f, 99.283f };
	float m_BindCameraYaw = 3.11021948f;
	float m_BindCameraPitch = 43.9585144f;
	float m_BindCameraRoll = 0.0f;
	// 처음 캐릭터 포지션 세팅
	TVector3 m_GunManPos = { 0.0f, 50.0f, 0.0f };
	TVector3 m_SwordManPos = { -50.0f, 50.0f, 0.0f };
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
	void InitializeSkyBox();
	void InitializeMap();
	void InitializeModel();
	void InitializeWeapon();
	void UpdateWeaponPosition();
	void InitializeLighting();
	ID3D11Buffer* CreateConstantBuffer(ID3D11Device* pd3dDevice, void* data, UINT iNumIndex, UINT iSize, bool bDynamic = false);
	void UpdateOBB();
public:
	SelectScene(LScene* parent);
	virtual ~SelectScene();
};


