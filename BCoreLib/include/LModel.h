#pragma once
#include "LFbxObj.h"
#include "LBox.h"

class LModel
{
public:
	static std::vector<LBoneWorld> m_texBoneArray;
	int m_InstanceSize = 0;
	int m_matBoneArraySize = 100;
	ComPtr<ID3D11Texture2D> pAnimationArrayTex = nullptr;
	ComPtr<ID3D11ShaderResourceView> pAnimationArraySRV = nullptr;
	std::vector<int> m_texAnimationOffset;
	std::vector<int> m_AnimationFrameList;
	int m_offsetCount = 0;
	ComPtr<ID3D11Buffer> m_pCurrentFrameCB;
	int m_CurrentFrame[4] = { 0, };
	static int m_AllAnimationFrame;
	T_BOX m_SettingBox;
	LBox m_OBBBox;
	T_BOX m_SettingBoxRightHand;
	LBox m_OBBBoxRightHand;
public:
	LFbxObj* m_pModel = nullptr;
	LFbxObj* m_pActionModel = nullptr;
	LFbxObj* m_pComparePtr = nullptr;
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
	float m_HP = 100.0f;
public:
	virtual void SetAnimationArrayTexture();
	virtual void SetAnimationArraySRV();
	virtual void SetInstancing(bool IsIntancing, int instancingCount);
	virtual void SetLFbxObj(LFbxObj* fbxObj);
	virtual void SetOBBBox(TVector3 min, TVector3 max, float boxScale);
	virtual void SetOBBBoxRightHand(TVector3 min, TVector3 max, float boxScale);
	virtual LFbxObj* GetLFbxObj();
public:
	virtual bool Init(int animationIndex);
	virtual bool ComputeOffset();
	virtual bool SetTexture();
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
	bool CreateCurrentFrameBuffer();
	bool Frame() override;
	bool AniFrame();
	bool Render() override;
	bool AniRender();
	bool Release() override;
};