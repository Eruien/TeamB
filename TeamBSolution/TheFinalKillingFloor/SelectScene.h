#pragma once
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
public:
	SelectScene(LScene* parent);
	virtual ~SelectScene();
};


