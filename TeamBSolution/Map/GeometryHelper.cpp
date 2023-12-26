#include "pch.h"
#include "GeometryHelper.h"
#include "Geometry.hpp"
//
//void GeometryHelper::CreateRectangle(std::shared_ptr<Geometry<VertexColorData>> geometry, Color color)
//{
//	std::vector<VertexColorData> vertices =
//	{
//		VertexColorData(TVector3(-0.5f, -0.5f, 0.0f), color),
//		VertexColorData(TVector3(-0.5f, 0.5f, 0.0f), color),
//		VertexColorData(TVector3(0.5f, 0.5f, 0.0f), color),
//		VertexColorData(TVector3(0.5f, -0.5f, 0.0f), color)
//	};
//	geometry->SetVertices(vertices);
//	std::vector<unsigned int> indices =
//	{
//		0, 1, 2,
//		0, 2, 3
//	};
//	geometry->SetIndices(indices);
//}
//void GeometryHelper::CreateRectangle(std::shared_ptr<Geometry<VertexTextureData>> geometry)
//{
//std::vector<VertexTextureData> vertices =
//	{
//		VertexTextureData(TVector3(-0.5f, -0.5f, 0.0f), TVector2(0.0f, 1.0f)),
//		VertexTextureData(TVector3(-0.5f, 0.5f, 0.0f), TVector2(0.0f, 0.0f)),
//		VertexTextureData(TVector3(0.5f, 0.5f, 0.0f), TVector2(1.0f, 0.0f)),
//		VertexTextureData(TVector3(0.5f, -0.5f, 0.0f), TVector2(1.0f, 1.0f))
//	};
//	geometry->SetVertices(vertices);
//
//	std::vector<unsigned int> indices =
//	{
//		0, 1, 2,
//		0, 2, 3
//	};
//	geometry->SetIndices(indices);
//}
//void GeometryHelper::CreateCube(std::shared_ptr<Geometry<VertexTextureData>> geometry)
//{
//	float w2 = 0.5f;
//	float h2 = 0.5f;
//	float d2 = 0.5f;
//
//	std::vector<VertexTextureData> vtx(24);
//
//	// æ’∏È
//	vtx[0] = VertexTextureData{ TVector3(-w2, -h2, -d2), TVector2(0.0f, 1.0f) };
//	vtx[1] = VertexTextureData{ TVector3(-w2, +h2, -d2), TVector2(0.0f, 0.0f) };
//	vtx[2] = VertexTextureData{ TVector3(+w2, +h2, -d2), TVector2(1.0f, 0.0f) };
//	vtx[3] = VertexTextureData{ TVector3(+w2, -h2, -d2), TVector2(1.0f, 1.0f) };
//	// µﬁ∏È
//	vtx[4] = VertexTextureData{ TVector3(-w2, -h2, +d2), TVector2(1.0f, 1.0f) };
//	vtx[5] = VertexTextureData{ TVector3(+w2, -h2, +d2), TVector2(0.0f, 1.0f) };
//	vtx[6] = VertexTextureData{ TVector3(+w2, +h2, +d2), TVector2(0.0f, 0.0f) };
//	vtx[7] = VertexTextureData{ TVector3(-w2, +h2, +d2), TVector2(1.0f, 0.0f) };
//	// ¿≠∏È
//	vtx[8] = VertexTextureData{ TVector3(-w2, +h2, -d2), TVector2(0.0f, 1.0f) };
//	vtx[9] = VertexTextureData{ TVector3(-w2, +h2, +d2), TVector2(0.0f, 0.0f) };
//	vtx[10] = VertexTextureData{ TVector3(+w2, +h2, +d2), TVector2(1.0f, 0.0f) };
//	vtx[11] = VertexTextureData{ TVector3(+w2, +h2, -d2), TVector2(1.0f, 1.0f) };
//	// æ∆∑ß∏È
//	vtx[12] = VertexTextureData{ TVector3(-w2, -h2, -d2), TVector2(1.0f, 1.0f) };
//	vtx[13] = VertexTextureData{ TVector3(+w2, -h2, -d2), TVector2(0.0f, 1.0f) };
//	vtx[14] = VertexTextureData{ TVector3(+w2, -h2, +d2), TVector2(0.0f, 0.0f) };
//	vtx[15] = VertexTextureData{ TVector3(-w2, -h2, +d2), TVector2(1.0f, 0.0f) };
//	// øﬁ¬ ∏È
//	vtx[16] = VertexTextureData{ TVector3(-w2, -h2, +d2), TVector2(0.0f, 1.0f) };
//	vtx[17] = VertexTextureData{ TVector3(-w2, +h2, +d2), TVector2(0.0f, 0.0f) };
//	vtx[18] = VertexTextureData{ TVector3(-w2, +h2, -d2), TVector2(1.0f, 0.0f) };
//	vtx[19] = VertexTextureData{ TVector3(-w2, -h2, -d2), TVector2(1.0f, 1.0f) };
//	// ø¿∏•¬ ∏È
//	vtx[20] = VertexTextureData{ TVector3(+w2, -h2, -d2), TVector2(0.0f, 1.0f) };
//	vtx[21] = VertexTextureData{ TVector3(+w2, +h2, -d2), TVector2(0.0f, 0.0f) };
//	vtx[22] = VertexTextureData{ TVector3(+w2, +h2, +d2), TVector2(1.0f, 0.0f) };
//	vtx[23] = VertexTextureData{ TVector3(+w2, -h2, +d2), TVector2(1.0f, 1.0f) };
//
//	geometry->SetVertices(vtx);
//
//	std::vector<UINT> idx(36);
//
//	// æ’∏È
//	idx[0] = 0; idx[1] = 1; idx[2] = 2;
//	idx[3] = 0; idx[4] = 2; idx[5] = 3;
//	// µﬁ∏È
//	idx[6] = 4; idx[7] = 5; idx[8] = 6;
//	idx[9] = 4; idx[10] = 6; idx[11] = 7;
//	// ¿≠∏È
//	idx[12] = 8; idx[13] = 9; idx[14] = 10;
//	idx[15] = 8; idx[16] = 10; idx[17] = 11;
//	// æ∆∑ß∏È
//	idx[18] = 12; idx[19] = 13; idx[20] = 14;
//	idx[21] = 12; idx[22] = 14; idx[23] = 15;
//	// øﬁ¬ ∏È
//	idx[24] = 16; idx[25] = 17; idx[26] = 18;
//	idx[27] = 16; idx[28] = 18; idx[29] = 19;
//	// ø¿∏•¬ ∏È
//	idx[30] = 20; idx[31] = 21; idx[32] = 22;
//	idx[33] = 20; idx[34] = 22; idx[35] = 23;
//
//	geometry->SetIndices(idx);
//}
//void GeometryHelper::CreateSphere(std::shared_ptr<Geometry<VertexTextureData>> geometry)
//{
//	float radius = 0.5f; // ±∏¿« π›¡ˆ∏ß
//	UINT stackCount = 5; // ∞°∑Œ ∫–«“
//	UINT sliceCount = 5; // ºº∑Œ ∫–«“
//
//	std::vector<VertexTextureData> vtx;
//
//	VertexTextureData v;
//
//	// ∫œ±ÿ
//	v.position = TVector3(0.0f, radius, 0.0f);
//	v.UV = TVector2(0.5f, 0.0f);
//	vtx.push_back(v);
//
//	float stackAngle = XM_PI / stackCount;
//	float sliceAngle = XM_2PI / sliceCount;
//
//	float deltaU = 1.f / static_cast<float>(sliceCount);
//	float deltaV = 1.f / static_cast<float>(stackCount);
//
//	// ∞Ì∏Æ∏∂¥Ÿ µπ∏Èº≠ ¡§¡°¿ª ∞ËªÍ«—¥Ÿ (∫œ±ÿ/≥≤±ÿ ¥‹¿œ¡°¿∫ ∞Ì∏Æ∞° X)
//	for (UINT y = 1; y <= stackCount - 1; ++y)
//	{
//		float phi = y * stackAngle;
//
//		// ∞Ì∏Æø° ¿ßƒ°«— ¡§¡°
//		for (UINT x = 0; x <= sliceCount; ++x)
//		{
//			float theta = x * sliceAngle;
//
//			v.position.x = radius * sinf(phi) * cosf(theta);
//			v.position.y = radius * cosf(phi);
//			v.position.z = radius * sinf(phi) * sinf(theta);
//
//			v.UV = TVector2(deltaU * x, deltaV * y);
//
//			vtx.push_back(v);
//		}
//	}
//
//	// ≥≤±ÿ
//	v.position = TVector3(0.0f, -radius, 0.0f);
//	v.UV = TVector2(0.5f, 1.0f);
//	vtx.push_back(v);
//
//	geometry->SetVertices(vtx);
//
//	std::vector<UINT> idx(36);
//
//	// ∫œ±ÿ ¿Œµ¶Ω∫
//	for (UINT i = 0; i <= sliceCount; ++i)
//	{
//		//  [0]
//		//   |  \
//		//  [i+1]-[i+2]
//		idx.push_back(0);
//		idx.push_back(i + 2);
//		idx.push_back(i + 1);
//	}
//
//	// ∏ˆ≈Î ¿Œµ¶Ω∫
//	UINT ringVertexCount = sliceCount + 1;
//	for (UINT y = 0; y < stackCount - 2; ++y)
//	{
//		for (UINT x = 0; x < sliceCount; ++x)
//		{
//			//  [y, x]-[y, x+1]
//			//  |		/
//			//  [y+1, x]
//			idx.push_back(1 + (y)*ringVertexCount + (x));
//			idx.push_back(1 + (y)*ringVertexCount + (x + 1));
//			idx.push_back(1 + (y + 1) * ringVertexCount + (x));
//			//		 [y, x+1]
//			//		 /	  |
//			//  [y+1, x]-[y+1, x+1]
//			idx.push_back(1 + (y + 1) * ringVertexCount + (x));
//			idx.push_back(1 + (y)*ringVertexCount + (x + 1));
//			idx.push_back(1 + (y + 1) * ringVertexCount + (x + 1));
//		}
//	}
//
//	// ≥≤±ÿ ¿Œµ¶Ω∫
//	UINT bottomIndex = static_cast<UINT>(vtx.size()) - 1;
//	UINT lastRingStartIndex = bottomIndex - ringVertexCount;
//	for (UINT i = 0; i < sliceCount; ++i)
//	{
//		//  [last+i]-[last+i+1]
//		//  |      /
//		//  [bottom]
//		idx.push_back(bottomIndex);
//		idx.push_back(lastRingStartIndex + i);
//		idx.push_back(lastRingStartIndex + i + 1);
//	}
//
//	geometry->SetIndices(idx);
//}
//void GeometryHelper::CreateGrid(std::shared_ptr<Geometry<VertexTextureData>> geometry, int sizeX, int sizeZ)
//{
//	std::vector<VertexTextureData> vtx;
//
//	for (int z = 0; z < sizeZ + 1; z++)
//	{
//		for (int x = 0; x < sizeX + 1; x++)
//		{
//			VertexTextureData v;
//			v.position = TVector3(static_cast<float>(x), 0, static_cast<float>(z));
//			v.UV = TVector2(static_cast<float>(x), static_cast<float>(z));
//
//			vtx.push_back(v);
//		}
//	}
//
//	geometry->SetVertices(vtx);
//
//	std::vector<UINT> idx;
//
//	for (int z = 0; z < sizeZ; z++)
//	{
//		for (int x = 0; x < sizeX; x++)
//		{
//			//  [0]
//			//   |	\
//			//  [2] - [1]
//			idx.push_back((sizeX + 1) * (z + 1) + (x));
//			idx.push_back((sizeX + 1) * (z)+(x + 1));
//			idx.push_back((sizeX + 1) * (z)+(x));
//			//  [1] - [2]
//			//   	\  |
//			//		  [0]
//			idx.push_back((sizeX + 1) * (z)+(x + 1));
//			idx.push_back((sizeX + 1) * (z + 1) + (x));
//			idx.push_back((sizeX + 1) * (z + 1) + (x + 1));
//		}
//	}
//
//	geometry->SetIndices(idx);
//}
//
//void GeometryHelper::CreateRectangle(std::shared_ptr<Geometry<VertexTextureNormalData>> geometry)
//{
//	std::vector<VertexTextureNormalData> vtx;
//	vtx.resize(4);
//
//	vtx[0].position = TVector3(-0.5f, -0.5f, 0.f);
//	vtx[0].uv = TVector2(0.f, 1.f);
//	vtx[0].normal = TVector3(0.f, 0.f, -1.f);
//	vtx[1].position = TVector3(-0.5f, 0.5f, 0.f);
//	vtx[1].uv = TVector2(0.f, 0.f);
//	vtx[1].normal = TVector3(0.f, 0.f, -1.f);
//	vtx[2].position = TVector3(0.5f, -0.5f, 0.f);
//	vtx[2].uv = TVector2(1.f, 1.f);
//	vtx[2].normal = TVector3(0.f, 0.f, -1.f);
//	vtx[3].position = TVector3(0.5f, 0.5f, 0.f);
//	vtx[3].uv = TVector2(1.f, 0.f);
//	vtx[3].normal = TVector3(0.f, 0.f, -1.f);
//	geometry->SetVertices(vtx);
//
//	std::vector<UINT> idx = { 0, 1, 2, 2, 1, 3 };
//	geometry->SetIndices(idx);
//}
//void GeometryHelper::CreateCube(std::shared_ptr<Geometry<VertexTextureNormalData>> geometry)
//{
//	float w2 = 0.5f;
//	float h2 = 0.5f;
//	float d2 = 0.5f;
//
//	std::vector<VertexTextureNormalData> vtx(24);
//
//	// æ’∏È
//	vtx[0] = VertexTextureNormalData(TVector3(-w2, -h2, -d2), TVector2(0.0f, 1.0f), TVector3(0.0f, 0.0f, -1.0f));
//	vtx[1] = VertexTextureNormalData(TVector3(-w2, +h2, -d2), TVector2(0.0f, 0.0f), TVector3(0.0f, 0.0f, -1.0f));
//	vtx[2] = VertexTextureNormalData(TVector3(+w2, +h2, -d2), TVector2(1.0f, 0.0f), TVector3(0.0f, 0.0f, -1.0f));
//	vtx[3] = VertexTextureNormalData(TVector3(+w2, -h2, -d2), TVector2(1.0f, 1.0f), TVector3(0.0f, 0.0f, -1.0f));
//	// µﬁ∏È
//	vtx[4] = VertexTextureNormalData(TVector3(-w2, -h2, +d2), TVector2(1.0f, 1.0f), TVector3(0.0f, 0.0f, 1.0f));
//	vtx[5] = VertexTextureNormalData(TVector3(+w2, -h2, +d2), TVector2(0.0f, 1.0f), TVector3(0.0f, 0.0f, 1.0f));
//	vtx[6] = VertexTextureNormalData(TVector3(+w2, +h2, +d2), TVector2(0.0f, 0.0f), TVector3(0.0f, 0.0f, 1.0f));
//	vtx[7] = VertexTextureNormalData(TVector3(-w2, +h2, +d2), TVector2(1.0f, 0.0f), TVector3(0.0f, 0.0f, 1.0f));
//	// ¿≠∏È
//	vtx[8] = VertexTextureNormalData(TVector3(-w2, +h2, -d2), TVector2(0.0f, 1.0f), TVector3(0.0f, 1.0f, 0.0f));
//	vtx[9] = VertexTextureNormalData(TVector3(-w2, +h2, +d2), TVector2(0.0f, 0.0f), TVector3(0.0f, 1.0f, 0.0f));
//	vtx[10] = VertexTextureNormalData(TVector3(+w2, +h2, +d2), TVector2(1.0f, 0.0f), TVector3(0.0f, 1.0f, 0.0f));
//	vtx[11] = VertexTextureNormalData(TVector3(+w2, +h2, -d2), TVector2(1.0f, 1.0f), TVector3(0.0f, 1.0f, 0.0f));
//	// æ∆∑ß∏È
//	vtx[12] = VertexTextureNormalData(TVector3(-w2, -h2, -d2), TVector2(1.0f, 1.0f), TVector3(0.0f, -1.0f, 0.0f));
//	vtx[13] = VertexTextureNormalData(TVector3(+w2, -h2, -d2), TVector2(0.0f, 1.0f), TVector3(0.0f, -1.0f, 0.0f));
//	vtx[14] = VertexTextureNormalData(TVector3(+w2, -h2, +d2), TVector2(0.0f, 0.0f), TVector3(0.0f, -1.0f, 0.0f));
//	vtx[15] = VertexTextureNormalData(TVector3(-w2, -h2, +d2), TVector2(1.0f, 0.0f), TVector3(0.0f, -1.0f, 0.0f));
//	// øﬁ¬ ∏È
//	vtx[16] = VertexTextureNormalData(TVector3(-w2, -h2, +d2), TVector2(0.0f, 1.0f), TVector3(-1.0f, 0.0f, 0.0f));
//	vtx[17] = VertexTextureNormalData(TVector3(-w2, +h2, +d2), TVector2(0.0f, 0.0f), TVector3(-1.0f, 0.0f, 0.0f));
//	vtx[18] = VertexTextureNormalData(TVector3(-w2, +h2, -d2), TVector2(1.0f, 0.0f), TVector3(-1.0f, 0.0f, 0.0f));
//	vtx[19] = VertexTextureNormalData(TVector3(-w2, -h2, -d2), TVector2(1.0f, 1.0f), TVector3(-1.0f, 0.0f, 0.0f));
//	// ø¿∏•¬ ∏È
//	vtx[20] = VertexTextureNormalData(TVector3(+w2, -h2, -d2), TVector2(0.0f, 1.0f), TVector3(1.0f, 0.0f, 0.0f));
//	vtx[21] = VertexTextureNormalData(TVector3(+w2, +h2, -d2), TVector2(0.0f, 0.0f), TVector3(1.0f, 0.0f, 0.0f));
//	vtx[22] = VertexTextureNormalData(TVector3(+w2, +h2, +d2), TVector2(1.0f, 0.0f), TVector3(1.0f, 0.0f, 0.0f));
//	vtx[23] = VertexTextureNormalData(TVector3(+w2, -h2, +d2), TVector2(1.0f, 1.0f), TVector3(1.0f, 0.0f, 0.0f));
//
//	geometry->SetVertices(vtx);
//
//	std::vector<UINT> idx(36);
//
//	// æ’∏È
//	idx[0] = 0; idx[1] = 1; idx[2] = 2;
//	idx[3] = 0; idx[4] = 2; idx[5] = 3;
//	// µﬁ∏È
//	idx[6] = 4; idx[7] = 5; idx[8] = 6;
//	idx[9] = 4; idx[10] = 6; idx[11] = 7;
//	// ¿≠∏È
//	idx[12] = 8; idx[13] = 9; idx[14] = 10;
//	idx[15] = 8; idx[16] = 10; idx[17] = 11;
//	// æ∆∑ß∏È
//	idx[18] = 12; idx[19] = 13; idx[20] = 14;
//	idx[21] = 12; idx[22] = 14; idx[23] = 15;
//	// øﬁ¬ ∏È
//	idx[24] = 16; idx[25] = 17; idx[26] = 18;
//	idx[27] = 16; idx[28] = 18; idx[29] = 19;
//	// ø¿∏•¬ 
//	idx[30] = 20; idx[31] = 21; idx[32] = 22;
//	idx[33] = 20; idx[34] = 22; idx[35] = 23;
//
//	geometry->SetIndices(idx);
//}
//void GeometryHelper::CreateGrid(std::shared_ptr<Geometry<VertexTextureNormalData>> geometry, int sizeX, int sizeZ)
//{
//	std::vector<VertexTextureNormalData> vtx;
//
//	for (int z = 0; z < sizeZ + 1; z++)
//	{
//		for (int x = 0; x < sizeX + 1; x++)
//		{
//			VertexTextureNormalData v;
//			v.position = TVector3(static_cast<float>(x), 0, static_cast<float>(z));
//			v.uv = TVector2(static_cast<float>(x), static_cast<float>(sizeZ - z));
//			v.normal = TVector3(0.f, 1.f, 0.f);
//
//			vtx.push_back(v);
//		}
//	}
//
//	geometry->SetVertices(vtx);
//
//	std::vector<UINT> idx;
//
//	for (int z = 0; z < sizeZ; z++)
//	{
//		for (int x = 0; x < sizeX; x++)
//		{
//			//  [0]
//			//   |	\
//			//  [2] - [1]
//			idx.push_back((sizeX + 1) * (z + 1) + (x));
//			idx.push_back((sizeX + 1) * (z)+(x + 1));
//			idx.push_back((sizeX + 1) * (z)+(x));
//			//  [1] - [2]
//			//   	\  |
//			//		  [0]
//			idx.push_back((sizeX + 1) * (z)+(x + 1));
//			idx.push_back((sizeX + 1) * (z + 1) + (x));
//			idx.push_back((sizeX + 1) * (z + 1) + (x + 1));
//		}
//	}
//
//	geometry->SetIndices(idx);
//}
//void GeometryHelper::CreateSphere(std::shared_ptr<Geometry<VertexTextureNormalData>> geometry)
//{
//	float radius = 0.5f; // ±∏¿« π›¡ˆ∏ß
//	UINT stackCount = 20; // ∞°∑Œ ∫–«“
//	UINT sliceCount = 20; // ºº∑Œ ∫–«“
//
//	std::vector<VertexTextureNormalData> vtx;
//
//	VertexTextureNormalData v;
//
//	// ∫œ±ÿ
//	v.position = TVector3(0.0f, radius, 0.0f);
//	v.uv = TVector2(0.5f, 0.0f);
//	v.normal = v.position;
//	v.normal.Normalize();
//	vtx.push_back(v);
//
//	float stackAngle = XM_PI / stackCount;
//	float sliceAngle = XM_2PI / sliceCount;
//
//	float deltaU = 1.f / static_cast<float>(sliceCount);
//	float deltaV = 1.f / static_cast<float>(stackCount);
//
//	// ∞Ì∏Æ∏∂¥Ÿ µπ∏Èº≠ ¡§¡°¿ª ∞ËªÍ«—¥Ÿ (∫œ±ÿ/≥≤±ÿ ¥‹¿œ¡°¿∫ ∞Ì∏Æ∞° X)
//	for (UINT y = 1; y <= stackCount - 1; ++y)
//	{
//		float phi = y * stackAngle;
//
//		// ∞Ì∏Æø° ¿ßƒ°«— ¡§¡°
//		for (UINT x = 0; x <= sliceCount; ++x)
//		{
//			float theta = x * sliceAngle;
//
//			v.position.x = radius * sinf(phi) * cosf(theta);
//			v.position.y = radius * cosf(phi);
//			v.position.z = radius * sinf(phi) * sinf(theta);
//
//			v.uv = TVector2(deltaU * x, deltaV * y);
//
//			v.normal = v.position;
//			v.normal.Normalize();
//
//			vtx.push_back(v);
//		}
//	}
//
//	// ≥≤±ÿ
//	v.position = TVector3(0.0f, -radius, 0.0f);
//	v.uv = TVector2(0.5f, 1.0f);
//	v.normal = v.position;
//	v.normal.Normalize();
//	vtx.push_back(v);
//
//	geometry->SetVertices(vtx);
//
//	std::vector<UINT> idx(36);
//
//	// ∫œ±ÿ ¿Œµ¶Ω∫
//	for (UINT i = 0; i <= sliceCount; ++i)
//	{
//		//  [0]
//		//   |  \
//		//  [i+1]-[i+2]
//		idx.push_back(0);
//		idx.push_back(i + 2);
//		idx.push_back(i + 1);
//	}
//
//	// ∏ˆ≈Î ¿Œµ¶Ω∫
//	UINT ringVertexCount = sliceCount + 1;
//	for (UINT y = 0; y < stackCount - 2; ++y)
//	{
//		for (UINT x = 0; x < sliceCount; ++x)
//		{
//			//  [y, x]-[y, x+1]
//			//  |		/
//			//  [y+1, x]
//			idx.push_back(1 + (y)*ringVertexCount + (x));
//			idx.push_back(1 + (y)*ringVertexCount + (x + 1));
//			idx.push_back(1 + (y + 1) * ringVertexCount + (x));
//			//		 [y, x+1]
//			//		 /	  |
//			//  [y+1, x]-[y+1, x+1]
//			idx.push_back(1 + (y + 1) * ringVertexCount + (x));
//			idx.push_back(1 + (y)*ringVertexCount + (x + 1));
//			idx.push_back(1 + (y + 1) * ringVertexCount + (x + 1));
//		}
//	}
//
//	// ≥≤±ÿ ¿Œµ¶Ω∫
//	UINT bottomIndex = static_cast<UINT>(vtx.size()) - 1;
//	UINT lastRingStartIndex = bottomIndex - ringVertexCount;
//	for (UINT i = 0; i < sliceCount; ++i)
//	{
//		//  [last+i]-[last+i+1]
//		//  |      /
//		//  [bottom]
//		idx.push_back(bottomIndex);
//		idx.push_back(lastRingStartIndex + i);
//		idx.push_back(lastRingStartIndex + i + 1);
//	}
//
//	geometry->SetIndices(idx);
//}
//


void GeometryHelper::CreateRectangle(std::shared_ptr<Geometry<VertexTextureNormalTangentData>> geometry)
{
	std::vector<VertexTextureNormalTangentData> vtx;
	vtx.resize(4);

	vtx[0].position = TVector3(-0.5f, -0.5f, 0.f);
	vtx[0].uv = TVector2(0.f, 1.f);
	vtx[0].normal = TVector3(0.f, 0.f, -1.f);
	vtx[0].tangent = TVector3(1.0f, 0.0f, 0.0f);
	vtx[1].position = TVector3(-0.5f, 0.5f, 0.f);
	vtx[1].uv = TVector2(0.f, 0.f);
	vtx[1].normal = TVector3(0.f, 0.f, -1.f);
	vtx[1].tangent = TVector3(1.0f, 0.0f, 0.0f);
	vtx[2].position = TVector3(0.5f, -0.5f, 0.f);
	vtx[2].uv = TVector2(1.f, 1.f);
	vtx[2].normal = TVector3(0.f, 0.f, -1.f);
	vtx[2].tangent = TVector3(1.0f, 0.0f, 0.0f);
	vtx[3].position = TVector3(0.5f, 0.5f, 0.f);
	vtx[3].uv = TVector2(1.f, 0.f);
	vtx[3].normal = TVector3(0.f, 0.f, -1.f);
	vtx[3].tangent = TVector3(1.0f, 0.0f, 0.0f);
	geometry->SetVertices(vtx);

	std::vector<UINT> idx = { 0, 1, 2, 2, 1, 3 };
	geometry->SetIndices(idx);
}
void GeometryHelper::CreateCube(std::shared_ptr<Geometry<VertexTextureNormalTangentData>> geometry)
{
	float w2 = 0.5f;
	float h2 = 0.5f;
	float d2 = 0.5f;

	std::vector<VertexTextureNormalTangentData> vtx(24);

	// æ’∏È
	vtx[0] = VertexTextureNormalTangentData(TVector3(-w2, -h2, -d2), TVector2(0.0f, 1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector3(1.0f, 0.0f, 0.0f));
	vtx[1] = VertexTextureNormalTangentData(TVector3(-w2, +h2, -d2), TVector2(0.0f, 0.0f), TVector3(0.0f, 0.0f, -1.0f), TVector3(1.0f, 0.0f, 0.0f));
	vtx[2] = VertexTextureNormalTangentData(TVector3(+w2, +h2, -d2), TVector2(1.0f, 0.0f), TVector3(0.0f, 0.0f, -1.0f), TVector3(1.0f, 0.0f, 0.0f));
	vtx[3] = VertexTextureNormalTangentData(TVector3(+w2, -h2, -d2), TVector2(1.0f, 1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector3(1.0f, 0.0f, 0.0f));
	// µﬁ∏È
	vtx[4] = VertexTextureNormalTangentData(TVector3(-w2, -h2, +d2), TVector2(1.0f, 1.0f), TVector3(0.0f, 0.0f, 1.0f), TVector3(-1.0f, 0.0f, 0.0f));
	vtx[5] = VertexTextureNormalTangentData(TVector3(+w2, -h2, +d2), TVector2(0.0f, 1.0f), TVector3(0.0f, 0.0f, 1.0f), TVector3(-1.0f, 0.0f, 0.0f));
	vtx[6] = VertexTextureNormalTangentData(TVector3(+w2, +h2, +d2), TVector2(0.0f, 0.0f), TVector3(0.0f, 0.0f, 1.0f), TVector3(-1.0f, 0.0f, 0.0f));
	vtx[7] = VertexTextureNormalTangentData(TVector3(-w2, +h2, +d2), TVector2(1.0f, 0.0f), TVector3(0.0f, 0.0f, 1.0f), TVector3(-1.0f, 0.0f, 0.0f));
	// ¿≠∏È
	vtx[8] = VertexTextureNormalTangentData(TVector3(-w2, +h2, -d2), TVector2(0.0f, 1.0f), TVector3(0.0f, 1.0f, 0.0f), TVector3(1.0f, 0.0f, 0.0f));
	vtx[9] = VertexTextureNormalTangentData(TVector3(-w2, +h2, +d2), TVector2(0.0f, 0.0f), TVector3(0.0f, 1.0f, 0.0f), TVector3(1.0f, 0.0f, 0.0f));
	vtx[10] = VertexTextureNormalTangentData(TVector3(+w2, +h2, +d2), TVector2(1.0f, 0.0f), TVector3(0.0f, 1.0f, 0.0f), TVector3(1.0f, 0.0f, 0.0f));
	vtx[11] = VertexTextureNormalTangentData(TVector3(+w2, +h2, -d2), TVector2(1.0f, 1.0f), TVector3(0.0f, 1.0f, 0.0f), TVector3(1.0f, 0.0f, 0.0f));
	// æ∆∑ß∏È
	vtx[12] = VertexTextureNormalTangentData(TVector3(-w2, -h2, -d2), TVector2(1.0f, 1.0f), TVector3(0.0f, -1.0f, 0.0f), TVector3(-1.0f, 0.0f, 0.0f));
	vtx[13] = VertexTextureNormalTangentData(TVector3(+w2, -h2, -d2), TVector2(0.0f, 1.0f), TVector3(0.0f, -1.0f, 0.0f), TVector3(-1.0f, 0.0f, 0.0f));
	vtx[14] = VertexTextureNormalTangentData(TVector3(+w2, -h2, +d2), TVector2(0.0f, 0.0f), TVector3(0.0f, -1.0f, 0.0f), TVector3(-1.0f, 0.0f, 0.0f));
	vtx[15] = VertexTextureNormalTangentData(TVector3(-w2, -h2, +d2), TVector2(1.0f, 0.0f), TVector3(0.0f, -1.0f, 0.0f), TVector3(-1.0f, 0.0f, 0.0f));
	// øﬁ¬ ∏È
	vtx[16] = VertexTextureNormalTangentData(TVector3(-w2, -h2, +d2), TVector2(0.0f, 1.0f), TVector3(-1.0f, 0.0f, 0.0f), TVector3(0.0f, 0.0f, -1.0f));
	vtx[17] = VertexTextureNormalTangentData(TVector3(-w2, +h2, +d2), TVector2(0.0f, 0.0f), TVector3(-1.0f, 0.0f, 0.0f), TVector3(0.0f, 0.0f, -1.0f));
	vtx[18] = VertexTextureNormalTangentData(TVector3(-w2, +h2, -d2), TVector2(1.0f, 0.0f), TVector3(-1.0f, 0.0f, 0.0f), TVector3(0.0f, 0.0f, -1.0f));
	vtx[19] = VertexTextureNormalTangentData(TVector3(-w2, -h2, -d2), TVector2(1.0f, 1.0f), TVector3(-1.0f, 0.0f, 0.0f), TVector3(0.0f, 0.0f, -1.0f));
	// ø¿∏•¬ ∏È
	vtx[20] = VertexTextureNormalTangentData(TVector3(+w2, -h2, -d2), TVector2(0.0f, 1.0f), TVector3(1.0f, 0.0f, 0.0f), TVector3(0.0f, 0.0f, 1.0f));
	vtx[21] = VertexTextureNormalTangentData(TVector3(+w2, +h2, -d2), TVector2(0.0f, 0.0f), TVector3(1.0f, 0.0f, 0.0f), TVector3(0.0f, 0.0f, 1.0f));
	vtx[22] = VertexTextureNormalTangentData(TVector3(+w2, +h2, +d2), TVector2(1.0f, 0.0f), TVector3(1.0f, 0.0f, 0.0f), TVector3(0.0f, 0.0f, 1.0f));
	vtx[23] = VertexTextureNormalTangentData(TVector3(+w2, -h2, +d2), TVector2(1.0f, 1.0f), TVector3(1.0f, 0.0f, 0.0f), TVector3(0.0f, 0.0f, 1.0f));

	geometry->SetVertices(vtx);

	std::vector<UINT> idx(36);

	// æ’∏È
	idx[0] = 0; idx[1] = 1; idx[2] = 2;
	idx[3] = 0; idx[4] = 2; idx[5] = 3;
	// µﬁ∏È
	idx[6] = 4; idx[7] = 5; idx[8] = 6;
	idx[9] = 4; idx[10] = 6; idx[11] = 7;
	// ¿≠∏È
	idx[12] = 8; idx[13] = 9; idx[14] = 10;
	idx[15] = 8; idx[16] = 10; idx[17] = 11;
	// æ∆∑ß∏È
	idx[18] = 12; idx[19] = 13; idx[20] = 14;
	idx[21] = 12; idx[22] = 14; idx[23] = 15;
	// øﬁ¬ ∏È
	idx[24] = 16; idx[25] = 17; idx[26] = 18;
	idx[27] = 16; idx[28] = 18; idx[29] = 19;
	// ø¿∏•¬ 
	idx[30] = 20; idx[31] = 21; idx[32] = 22;
	idx[33] = 20; idx[34] = 22; idx[35] = 23;

	geometry->SetIndices(idx);
}
void GeometryHelper::CreateGrid(std::shared_ptr<Geometry<VertexTextureNormalTangentData>> geometry, int sizeX, int sizeZ)
{
	std::vector<VertexTextureNormalTangentData> vtx;

	for (int z = 0; z < sizeZ + 1; z++)
	{
		for (int x = 0; x < sizeX + 1; x++)
		{
			VertexTextureNormalTangentData v;
			v.position = TVector3(static_cast<float>(x), 0, static_cast<float>(z));
			v.uv = TVector2(static_cast<float>(x), static_cast<float>(sizeZ - z));
			v.normal = TVector3(0.f, 1.f, 0.f);
			v.tangent = TVector3(1.f, 0.f, 0.f);

			vtx.push_back(v);
		}
	}

	geometry->SetVertices(vtx);

	std::vector<UINT> idx;

	for (int z = 0; z < sizeZ; z++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			//  [0]
			//   |	\
			//  [2] - [1]
			idx.push_back((sizeX + 1) * (z + 1) + (x));
			idx.push_back((sizeX + 1) * (z)+(x + 1));
			idx.push_back((sizeX + 1) * (z)+(x));
			//  [1] - [2]
			//   	\  |
			//		  [0]
			idx.push_back((sizeX + 1) * (z)+(x + 1));
			idx.push_back((sizeX + 1) * (z + 1) + (x));
			idx.push_back((sizeX + 1) * (z + 1) + (x + 1));
		}
	}

	geometry->SetIndices(idx);
}
void GeometryHelper::CreateSphere(std::shared_ptr<Geometry<VertexTextureNormalTangentData>> geometry)
{
	float radius = 0.5f; // ±∏¿« π›¡ˆ∏ß
	UINT stackCount = 20; // ∞°∑Œ ∫–«“
	UINT sliceCount = 20; // ºº∑Œ ∫–«“

	std::vector<VertexTextureNormalTangentData> vtx;

	VertexTextureNormalTangentData v;

	// ∫œ±ÿ
	v.position = TVector3(0.0f, radius, 0.0f);
	v.uv = TVector2(0.5f, 0.0f);
	v.normal = v.position;
	v.normal.Normalize();
	v.tangent = TVector3(1.f, 0.f, 0.f);
	v.tangent.Normalize();
	vtx.push_back(v);

	float stackAngle = XM_PI / stackCount;
	float sliceAngle = XM_2PI / sliceCount;

	float deltaU = 1.f / static_cast<float>(sliceCount);
	float deltaV = 1.f / static_cast<float>(stackCount);

	// ∞Ì∏Æ∏∂¥Ÿ µπ∏Èº≠ ¡§¡°¿ª ∞ËªÍ«—¥Ÿ (∫œ±ÿ/≥≤±ÿ ¥‹¿œ¡°¿∫ ∞Ì∏Æ∞° X)
	for (UINT y = 1; y <= stackCount - 1; ++y)
	{
		float phi = y * stackAngle;

		// ∞Ì∏Æø° ¿ßƒ°«— ¡§¡°
		for (UINT x = 0; x <= sliceCount; ++x)
		{
			float theta = x * sliceAngle;

			v.position.x = radius * sinf(phi) * cosf(theta);
			v.position.y = radius * cosf(phi);
			v.position.z = radius * sinf(phi) * sinf(theta);

			v.uv = TVector2(deltaU * x, deltaV * y);

			v.normal = v.position;
			v.normal.Normalize();

			v.tangent.x = -radius * sinf(phi) * sinf(theta);
			v.tangent.y = 0.0f;
			v.tangent.z = radius * sinf(phi) * cosf(theta);
			v.tangent.Normalize();

			vtx.push_back(v);
		}
	}

	// ≥≤±ÿ
	v.position = TVector3(0.0f, -radius, 0.0f);
	v.uv = TVector2(0.5f, 1.0f);
	v.normal = v.position;
	v.normal.Normalize();
	v.tangent = TVector3(1.f, 0.f, 0.f);
	v.tangent.Normalize();
	vtx.push_back(v);

	geometry->SetVertices(vtx);

	std::vector<UINT> idx(36);

	// ∫œ±ÿ ¿Œµ¶Ω∫
	for (UINT i = 0; i <= sliceCount; ++i)
	{
		//  [0]
		//   |  \
		//  [i+1]-[i+2]
		idx.push_back(0);
		idx.push_back(i + 2);
		idx.push_back(i + 1);
	}

	// ∏ˆ≈Î ¿Œµ¶Ω∫
	UINT ringVertexCount = sliceCount + 1;
	for (UINT y = 0; y < stackCount - 2; ++y)
	{
		for (UINT x = 0; x < sliceCount; ++x)
		{
			//  [y, x]-[y, x+1]
			//  |		/
			//  [y+1, x]
			idx.push_back(1 + (y)*ringVertexCount + (x));
			idx.push_back(1 + (y)*ringVertexCount + (x + 1));
			idx.push_back(1 + (y + 1) * ringVertexCount + (x));
			//		 [y, x+1]
			//		 /	  |
			//  [y+1, x]-[y+1, x+1]
			idx.push_back(1 + (y + 1) * ringVertexCount + (x));
			idx.push_back(1 + (y)*ringVertexCount + (x + 1));
			idx.push_back(1 + (y + 1) * ringVertexCount + (x + 1));
		}
	}

	// ≥≤±ÿ ¿Œµ¶Ω∫
	UINT bottomIndex = static_cast<UINT>(vtx.size()) - 1;
	UINT lastRingStartIndex = bottomIndex - ringVertexCount;
	for (UINT i = 0; i < sliceCount; ++i)
	{
		//  [last+i]-[last+i+1]
		//  |      /
		//  [bottom]
		idx.push_back(bottomIndex);
		idx.push_back(lastRingStartIndex + i);
		idx.push_back(lastRingStartIndex + i + 1);
	}

	geometry->SetIndices(idx);
}