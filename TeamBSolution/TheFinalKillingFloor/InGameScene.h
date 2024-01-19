#pragma once
#include "LScene.h"
#include "LMap.h"
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

struct SHADOW_CONSTANT_BUFFER
{
	TMatrix	g_matShadow;
};

class InGameScene : public SceneState
{
public:
	std::shared_ptr<LDebugCamera> m_DebugCamera = nullptr;
	std::shared_ptr<LModelCamera> m_ModelCamera = nullptr;
	std::shared_ptr<LSkyBox> m_SkyBox = nullptr;
public:
	std::shared_ptr<LMap> m_CustomMap = nullptr;
	std::shared_ptr<LPlayer> m_PlayerModel = nullptr;
	std::vector<LNPC*> m_ZombieModelList;
	std::shared_ptr<LModel> m_MapModel = nullptr;
	LFbxObj* fbxObj = nullptr;
	LFbxObj* zombieObj = nullptr;
	LFbxObj* mapObj = nullptr;
	int m_EnemySize = 10;
public:
	// Shadow
	LDxRT m_RT;
	LPlaneShape m_pQuad;
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
	std::vector<T_BOX*> m_BoxList;
	std::vector<LBox*> m_obbBoxList;
	TMatrix boxWorld;
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