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
	// change height map size to power of 2+ 1 ? // ���� ���� ũ�⸦ 2�� �ŵ������� 1�� ���� ������ ����
	bool CheckSquare(int n);
	int ResizeMap(int n);

public:
	void Init(UINT& rowNum, UINT& colNum, float heightScale, wstring heightTexPath);
	void SaveHeightMap(wstring savePath);
	float GetHeightByRowCol(int row, int col);
	float GetHeightByIndex(UINT index);
};

