#pragma once
/*************************
 * ���������� : 2023-12-04
 * �ۼ��� : ����
 * ���ϸ� : LModel.h
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
	// Mesh�� ������ ���� �����̶� Render�� ���� ���� pnct���� model�� �ű��
	void SetLFbxObj(LFbxObj* fbxObj);
	void SetChild();
	void SetAnimationSpeed(float speed);
	float GetAnimationSpeed();
	LFbxObj* GetLFbxObj();
	LMesh* GetMesh();
public:
	bool PostRender() override;
};