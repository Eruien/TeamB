#pragma once
#include "LScene.h"
#include "LMap.h"
#include "LHeightMap.h"
#include "LPlayer.h"
#include "LFbxMgr.h"
#include "LDebugCamera.h"
#include "LModelCamera.h"
#include "LDxRT.h"
#include "LPlaneShape.h"
#include "LNPC.h"
#include "LSkyBox.h"
#include "LSelect.h"
#include "LBox.h"
#include "KObject.h"
#include "ZombieWave.h"

struct SHADOW_CONSTANT_BUFFER
{
	TMatrix	g_matShadow;
};

class InGameScene : public SceneState
{
public:
	std::shared_ptr<LDebugCamera> m_DebugCamera = nullptr;
	std::shared_ptr<LModelCamera> m_ModelCamera = nullptr;
	std::shared_ptr<LCamera> m_MinimapCamera = nullptr;
	std::shared_ptr<LSkyBox> m_SkyBox = nullptr;
	std::shared_ptr<KObject> m_enemyHp;

public:
	std::shared_ptr<LHeightMap> m_CustomMap = nullptr;
	std::shared_ptr<LModel> m_GunModel = nullptr;
	std::vector<LNPC*> m_ZombieModelList;
	std::shared_ptr<LModel> m_MapModel = nullptr;
	LFbxObj* mapObj = nullptr;
	std::shared_ptr<ZombieWave> m_ZombieWave = nullptr;
	int m_WaveCount = 5;
public:
	// Shadow
	LDxRT m_RT;
	LDxRT m_rtMinimap;
	LPlaneShape m_pQuad;
	LPlaneShape m_ShapeMinimap;
	TMatrix	m_matWorld;
	TMatrix	m_matShadow;
	TMatrix	m_matViewLight;
	TMatrix	m_matProjLight;
	TMatrix	m_matTexture;
	SHADOW_CONSTANT_BUFFER m_CBmatShadow;
	ComPtr<ID3D11Buffer> m_pCBShadow;
public:
	// Collision
	LSelect* m_Select = nullptr;
public:
	shared_ptr<KObject> m_playerIcon;
public:
	bool Init() override;
	void Render() override;
	void Process() override;
	void Release();
public:
	void RenderObject();
	void RenderShadow(TMatrix* matWorld, TMatrix* matShadow,
		TMatrix* matView, TMatrix* matProj);
	void CreateShadowConstantBuffer();
public:
	InGameScene(LScene* parent);
	virtual ~InGameScene();

};