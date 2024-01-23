#pragma once
#include "LFbxObj.h"

class LModel
{
public:
	LFbxObj* m_pModel = nullptr;
	LFbxObj* m_pActionModel = nullptr;
public:
	float m_fCurrentAnimTime = 0.0f;
	LBoneWorld m_matBoneArray;
	LBoneWorld m_matMeshBoneArray;
	ComPtr<ID3D11Buffer> m_pBoneArrayCB = nullptr;
	TMatrix m_matControl;
	std::wstring m_ParentBoneName;
public:
	bool m_TimerStart = false;
	bool m_TimerEnd = false;
public:
	virtual void SetInstancing(bool IsIntancing, int instancingCount);
	virtual void SetLFbxObj(LFbxObj* fbxObj);
	virtual LFbxObj* GetLFbxObj();
public:
	virtual bool CreateBoneBuffer();
	virtual bool Frame();
	virtual bool FrameInstancing();
	virtual bool Render();
	bool ShadowRender(TMatrix* matShadowWorld, TMatrix* matShadowView, TMatrix* matShadowProj);
	virtual bool Release();
};

class LSkinningModel : public LModel
{
public:
	virtual bool AllNodeRender();
	bool CreateBoneBuffer() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
};