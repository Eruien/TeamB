#include "LFbxImport.h"

void LFbxImport::GetAnimation(LFbxObj* fbxObj)
{
	FbxTime::SetGlobalTimeMode(FbxTime::eFrames30);
	FbxAnimStack* stack = m_pFbxScene->GetSrcObject<FbxAnimStack>(0);

	if (stack == nullptr)
	{
		MessageBoxA(NULL, "AnimationStack is Null", "Error Box", MB_OK);
		return;
	}

	FbxString takeName = stack->GetName();
	FbxTakeInfo* takeInfo = m_pFbxScene->GetTakeInfo(takeName);
	FbxTimeSpan ts = takeInfo->mLocalTimeSpan;
	FbxTime startTime = ts.GetStart();
	FbxTime endTime = ts.GetStop();

	FbxTime::EMode timeMode = FbxTime::GetGlobalTimeMode();
	FbxLongLong startFrame = startTime.GetFrameCount(timeMode);
	FbxLongLong endFrame = endTime.GetFrameCount(timeMode);

	FbxTime time;

	for (int t = startFrame; t < endFrame; t++)
	{
		time.SetFrame(t, timeMode);

		for (int iMesh = 0; iMesh < fbxObj->m_MeshList.size(); iMesh++)
		{
			fbxObj->m_MeshList[iMesh]->m_iStartFrame = startFrame;
			fbxObj->m_MeshList[iMesh]->m_iEndFrame = endFrame;
			fbxObj->m_MeshList[iMesh]->m_iFrameSpeed = 30;
			fbxObj->m_MeshList[iMesh]->m_iTickForFrame = 160;

			// 초당 30프레임 기준으로 프레임1당 fbxMatrix
			// Mesh당 행렬을 얻는다.
			FbxAMatrix fbxMat = m_MeshNodeList[iMesh]->EvaluateGlobalTransform(time);
			TMatrix mat = ConvertAMatrix(fbxMat);
			mat = DxConvertMatrix(mat);
			fbxObj->m_MeshList[iMesh]->m_MatrixArray.push_back(mat);
		}
	}
}