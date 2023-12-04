#pragma once
/*************************
 * 최종수정일 : 2023-12-04
 * 작성자 : 시진
 * 파일명 : LModel.h
 *************************/

#include "LObject.h"
#include "LFbxObj.h"

class LModel : public LObject
{
	LFbxObj* m_LFbxObj = nullptr;
	std::shared_ptr<LMesh> m_Mesh = nullptr;
public:
	float m_AnimationSpanTime = 0.0f;
	float m_AnimationSpeed = 30.0f;
public:
	std::vector<std::shared_ptr<LModel>> m_SubModel;
public:
	// Mesh의 나머지 정보 세팅이랑 Render를 위한 세팅 pnct정점 model로 옮기기
	void SetLFbxObj(LFbxObj* fbxObj);
	void SetChild();
	void SetAnimationSpeed(float speed);
	float GetAnimationSpeed();
	LFbxObj* GetLFbxObj();
	LMesh* GetMesh();
public:
	bool PostRender() override;
};