#pragma once
#include "LNode.h"
#include "LObject.h"


class LQurdtree : public LObject
{
public:
	// map�� ����� node���� ����ϱ� ���� ��ҵ�
	LMap* m_pMap = nullptr;
	LCamera* m_pCamera = nullptr;
	LTexture* m_TexArray[5];

	DWORD m_Row = 0;
	DWORD m_Col = 0;
	DWORD m_RowCellSize = 0;
	DWORD m_ColCellSize = 0;
	std::vector<LNode*> m_NodeList;
	std::vector<LNode*> m_LeafNodeList;
	std::vector<std::vector<DWORD>> m_LeafIndexList;
public:
	LNode* m_RootNode = nullptr;
	int m_TreeDepth = 0;
	std::queue<LNode*> m_Queue;

	// for splatting
	BYTE* m_fAlphaData;
	ComPtr<ID3D11Texture2D> m_pMaskAlphaTex;
	ComPtr<ID3D11ShaderResourceView> m_pMaskAlphaTexSRV;
	HRESULT CreateAlphaTexture(ID3D11Device* pDevice, DWORD dwWidth, DWORD dwHeight);
	void Splatting(TVector3 vIntersection, UINT iSplattingTexIndex, float fSplattingRadius = 5.f);
	
public:
	void SetDepth(int depth);
public:
	LNode* NewNode(LNode* pParnet, DWORD topLeft, DWORD topRight, DWORD bottomRight, DWORD bottomLeft);
	void BuildTree(LNode* pNode);
	void BuildQurdTree(LMap* pMap, DWORD row, DWORD col);
	TVector2 GetHeightFormNode(LNode* pNode);
	void ComputeBoundingBox(LNode* pNode);
	void FindNode(LNode* pNode);
	void AddLeafNode(LNode* pNode);
	void UpdateIndexBuffer();

public: // �����߰� for picking
	UINT SelectVertexList(T_BOX tBox, vector<LNode*>& selectNodeList);
	// for splatting
public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
public:
	virtual ~LQurdtree();
};




