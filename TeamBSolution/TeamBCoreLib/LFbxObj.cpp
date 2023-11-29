#include "LFbxObj.h"

void LMesh::SetAnimation(int startFrame, int endFrame, int frameSpeed, int tickForFrame)
{
	m_iStartFrame = startFrame;
	m_iEndFrame = endFrame;
	m_iFrameSpeed = frameSpeed;
	m_iTickForFrame = tickForFrame;
}

void LMesh::SetDefaultFilePath(std::wstring filePath)
{
	m_DefaultFilePath = filePath;
}

void LMesh::LoadTexture(std::wstring texFileName)
{
	LTexture* tex = LManager<LTexture>::GetInstance().Load(texFileName.c_str());
	m_TexArray.push_back(tex);
}