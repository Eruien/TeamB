#pragma once
#include "LQurdTree.h"
#include "LStd.h"
#include "LCore.h"

using namespace std;

class LMapTileRender : public LQurdtree
{
public:
	LMap*					_pMap;
	PNCT_VERTEX*			_pVertexList;
	vector<DWORD>			_IndexList;
	ComPtr<ID3D11Buffer>	_pTileIndexBuffer;
	int						_iNumFace;
	int						_iMinSplitDistance; // 최소 분할 크기( 정점 간 간격 )
	DWORD					_dwWidth;
	DWORD					_dwHeight;
	bool					_bSharedVertexBuffer;  // 공유 정점 버퍼 사용
	ComPtr<ID3D11Buffer>	_pIndexBuffer;
	size_t					_dstibOffset;
	size_t					_ibOffset;
	

public:
	bool Build(LMap* pMap, DWORD dwWidth, DWORD dwHeight);
	LNode* CreateNode(LNode* pParentNode, DWORD dwTL, DWORD dwTR, DWORD dwBL, DWORD dwBR);
	TVector2 GetHeightFromNode(DWORD dwTL, DWORD dwTR, DWORD dwBL, DWORD dwBR);
	void ComputeBoundingBox(LNode* pNode);
	void UpdateBoundingBox(LNode* pNode);
	bool SubDivide(LNode* pNode);
	bool Release();
	bool Render(ID3D11DeviceContext* pContext);
	bool CreateVertexList(LNode* pNode);
	bool CreateIndexList(LNode* pNode);
	
public:
	BYTE* _fLookup = nullptr;
	ComPtr<ID3D11Texture2D> _pRoughnessLookUpTex;
	ComPtr<ID3D11ShaderResourceView> _pLookupSRV;
	HRESULT CreateAlphaTexture(ID3D11Device* pDevice, DWORD dwWidth, DWORD dwHeight);

	LMapTileRender(void);
	virtual ~LMapTileRender(void);
};

