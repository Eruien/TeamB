#pragma once

enum ShaderScope
{
	SS_None=0,
	SS_VertexShader = (1 << 0),						// 0001
	SS_PixelShader = (1 << 1),						// 0010
	SS_Both = SS_VertexShader | SS_PixelShader		// 0011 ( OR¿¬»ê )
};

enum class ComponentType
{
	Transform,
	MeshRenderer,
	ModelRenderer,
	Camera,
	Animator,
	Light,

	// temp for maptool // ?
	Terrain,
	FixedComponentCount,
	Script
};

enum class ProjectionType
{
	Perspective = 0,
	Orthographic = 1
};

enum class ResourceType
{
	Unknown = -1,
	Mesh,
	Shader,
	Texture,
	Material,
	Animation,

	RecoucesCount
};