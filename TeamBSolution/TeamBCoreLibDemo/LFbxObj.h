#pragma once
#include "LObject.h"

struct LMesh
{
public:
	int m_iStartFrame = 0;
	int m_iEndFrame = 0;
	int m_iFrameSpeed = 30;
	int m_iTickForFrame = 160;
public:
	int m_NumPoly = 0;
	TMatrix m_MatWorld;
	std::wstring m_DefaultFilePath;
public:
	std::vector<std::vector<SimpleVertex>> m_TriangleList;
	std::vector<UINT> m_TriangleOffsetList;
	std::vector<std::wstring> m_TexFileNameList;
	std::vector<LTexture*> m_TexArray;
	std::vector<TMatrix> m_MatrixArray;
public:
	void SetAnimation(int startFrame, int endFrame, int frameSpeed, int tickForFrame);
	void SetDefaultFilePath(std::wstring filePath);
	void LoadTexture(std::wstring texFileName);
};

class LFbxObj
{
public:
	std::vector<std::shared_ptr<LMesh>> m_MeshList;
};

