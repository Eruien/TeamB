#pragma once
#include "LStd.h"
#include "Collision.h"


struct VertexData
{
	TVector3 position = { 0, 0, 0 };

	VertexData() = default;
	VertexData(const TVector3& pos) : position(pos) {}
};

struct VertexTextureNormalData
{
	TVector3 position = { 0, 0, 0 };
	TVector3 normal = { 0, 0, 0 };
	TVector2 uv = { 0, 0 };

	VertexTextureNormalData() = default;
	VertexTextureNormalData(const TVector3& pos, const TVector2& uv, const TVector3& normal) : position(pos), uv(uv), normal(normal) {}
};

struct VertexTextureNormalTangentData
{
	TVector3 position = { 0, 0, 0 };
	TVector3 normal = { 0, 0, 0 };
	TVector2 uv = { 0, 0 };
	TVector3 tangent = { 0, 0, 0 };

	VertexTextureNormalTangentData() = default;
	VertexTextureNormalTangentData(const TVector3& pos, const TVector2& uv, const TVector3& normal, const TVector3& tangent) : position(pos), uv(uv), normal(normal), tangent(tangent) {}
};


struct PNCTVertex
{
	TVector3 position = { 0, 0, 0 };
	TVector3 normal = { 0, 0, 0 };
	TVector4 color = { 0, 0, 0, 0 };
	TVector2 uv = { 0, 0 };

	PNCTVertex() = default;
	PNCTVertex(const TVector3& pos, const TVector2& uv, const TVector4& color, const TVector3& normal) : position(pos), uv(uv), normal(normal), color(color) {}
};


struct VertexTextureNormalTangentBlendData
{
	TVector3 position = { 0, 0, 0 };
	TVector3 normal = { 0, 0, 0 };
	TVector2 uv = { 0, 0 };
	TVector3 tangent = { 0, 0, 0 };
	TVector4 blendIndices = { 0, 0, 0, 0 };
	TVector4 blendWeights = { 0, 0, 0, 0 };

	VertexTextureNormalTangentBlendData() = default;
	VertexTextureNormalTangentBlendData(const TVector3& pos, const TVector2& uv, const TVector3& normal, const TVector3& tangent, const TVector4 blendIndices, const TVector4 blendWeights)
		: position(pos), uv(uv), normal(normal), tangent(tangent), blendIndices(blendIndices), blendWeights(blendWeights) {}
};

struct VertexTextureData
{
	TVector3 position = { 0,0,0 };
	TVector2 UV = { 0,0 };

	VertexTextureData() = default;
	VertexTextureData(const TVector3& pos, const TVector2& uv) : position(pos), UV(uv) {}

	static std::vector<D3D11_INPUT_ELEMENT_DESC> descs;
};

struct VertexColorData
{
	TVector3 position = { 0,0,0 };
	Color color = { 0,0,0,0 };

	VertexColorData() = default;
	VertexColorData(const TVector3& pos, const Color& col) : position(pos), color(col) {}

	static std::vector<D3D11_INPUT_ELEMENT_DESC> descs;
};

using ModelVertexType = VertexTextureNormalTangentBlendData;