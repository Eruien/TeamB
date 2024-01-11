#pragma once
#include "LStd.h"
#include "LManagerAgr.h"
#include "LGlobal.h"
#include <algorithm>

struct SplattingDesc
{
	wstring alphaTexPath = L"";

	wstring texture1Path = L"";
	wstring texture2Path = L"";
	wstring texture3Path = L"";
	wstring texture4Path = L"";

	int rowNum = 0;
	int colNum = 0;
};

class Splatting
{
private:
	shared_ptr<LTexture> alphaTexture;
	shared_ptr<LTexture> texture1;
	shared_ptr<LTexture> texture2;
	shared_ptr<LTexture> texture3;
	shared_ptr<LTexture> texture4;

public:
	void TillingTexture(TVector3 centerPos, int tillingTexNum, vector<PNCT_VERTEX>& vertexList, vector<UINT>& updateIdxList);
	void SetVertexByTexture(vector<PNCT_VERTEX>& vertexList);
	void SaveAlphaTexture(wstring savePath);
	void SetSRV();

	void Init(SplattingDesc);
};

