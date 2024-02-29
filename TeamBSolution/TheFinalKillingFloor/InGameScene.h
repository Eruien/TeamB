#pragma once
#include "LScene.h"
#include "LMap.h"
#include "LHeightMap.h"
#include "LPlayer.h"
#include "LFbxMgr.h"
#include "LDebugCamera.h"
#include "LModelCamera.h"
#include "LBackView.h"
#include "LDxRT.h"
#include "LPlaneShape.h"
#include "LNPC.h"
#include "Tank.h"
#include "LSkyBox.h"
#include "LSelect.h"
#include "LBox.h"
#include "KObject.h"
#include "ZombieWave.h"
#include "LSoundMgr.h"
#include "Light.h" // light

const int g_iMaxLight = 1;


struct SHADOW_CONSTANT_BUFFER
{
	TMatrix	g_matShadow;
};

class InGameScene : public SceneState
{
public:
	std::shared_ptr<LDebugCamera> m_DebugCamera = nullptr;
	std::shared_ptr<LModelCamera> m_ModelCamera = nullptr;
	std::shared_ptr<LBackView> m_BackViewCamera = nullptr;
	std::shared_ptr<LCamera> m_MinimapCamera = nullptr;
	std::shared_ptr<LCamera> m_MinimapPosCamera = nullptr;
	std::shared_ptr<LSkyBox> m_SkyBox = nullptr;
	vector<shared_ptr<KObject>> _EnemyMarks;
	shared_ptr<KObject> m_muzzleFlash = nullptr;
	vector<shared_ptr<KObject>> m_bloodSplatter;
	int m_crrBlood = 0;
public:
	std::shared_ptr<LHeightMap> m_CustomMap = nullptr;
	std::shared_ptr<LModel> m_GunModel = nullptr;
	std::vector<LNPC*> m_ZombieModelList;
	std::vector<Tank*> m_TankList;
	std::vector<shared_ptr<LModel>> m_BulletList;
	std::vector<bool> m_VisibleBulletList;
	int BulletIndex = 0;
	std::shared_ptr<LModel> m_Tree;
	std::shared_ptr<LModel> m_MapModel = nullptr;
	vector<shared_ptr<LModel>> m_WallList;
	vector<shared_ptr<LModel>> m_TreeList;
	vector<shared_ptr<LModel>> m_GrassList;


	LFbxObj* mapObj = nullptr;
	LFbxObj* treeObj = nullptr;
	LFbxObj* bulletObj;
	LFbxObj* wallObj = nullptr;
	LFbxObj* grassObj = nullptr;

	std::shared_ptr<ZombieWave> m_ZombieWave = nullptr;
	int m_WaveCount = 5;
	int m_CurrentWave = 1;

public: // light
	LIGHT_CONSTANT_BUFFER1	m_cbLight1;
	LIGHT_CONSTANT_BUFFER2	m_cbLight2;
	ComPtr<ID3D11Buffer>	m_pConstantBufferLight[2];
	Light					m_PointLight[1];
	
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
	bool IsNextWave = false;
	bool IsReleaseTank = false;
	bool IsEndGame = false;
public:
	bool Init() override;
	void Render() override;
	void Process() override;
	void Release();
public:
	void NextWave();
	void RenderObject();
	void RenderShadow(TMatrix* matWorld, TMatrix* matShadow,
		TMatrix* matView, TMatrix* matProj);
	void CreateShadowConstantBuffer();
public:
	InGameScene(LScene* parent);
	virtual ~InGameScene();



	ID3D11Buffer* CreateConstantBuffer(ID3D11Device* pd3dDevice, void* data, UINT iNumIndex, UINT iSize, bool bDynamic = false);
};