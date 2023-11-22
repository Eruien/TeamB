#include "LCore.h"
#include "LHeightMap.h"
#include "LQurdTree.h"
#include "LGlobal.h"
#include "LDebugCamera.h"
#include "LFbxMgr.h"
#include "LModel.h"

class Sample : public LCore
{
public:
	ID3D11BlendState* m_AlphaBlend = nullptr;
	std::shared_ptr<LDebugCamera> m_DebugCamera = nullptr;
	LMap* m_CustomMap = nullptr;
	LHeightMap* m_HeightMap = nullptr;
	LQurdtree* m_Tree = nullptr;
	// fbxObj�� Manager���� �����ϱ� ������ �Ŵ����� �����Ǹ� �˾Ƽ� �޸𸮰� �����ɰ��̴�.
	std::shared_ptr<LModel> TurretModel = nullptr;
	std::shared_ptr<LModel> ZombieModel = nullptr;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	void CreateBlendState();
public:
	virtual ~Sample();
};

