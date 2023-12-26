#pragma once
#include "LStd.h"
#include "Texture.h"

class HeightMap
{
private:
	shared_ptr<Texture> heightMapTexture;

	UINT rowNum;
	UINT colNum;
	float heightScale = 0.f;

public:
	vector<float> heightList;
private:
	// change height map size to power of 2+ 1 ? // 높이 맵의 크기를 2의 거듭제곱에 1을 더한 값으로 변경
	bool CheckSquare(int n);
	int ResizeMap(int n);

public:
	void Init(UINT& rowNum, UINT& colNum, float heightScale, wstring heightTexPath);
	void SaveHeightMap(wstring savePath);
	float GetHeightByRowCol(int row, int col);
	float GetHeightByIndex(UINT index);
};

