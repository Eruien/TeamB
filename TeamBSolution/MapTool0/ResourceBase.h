#pragma once

#include "LStd.h"

enum class ResourceType
{
	Unknown = -1,
	Mesh,
	Shader,
	Texture,
	Material,
	Animation,

	RecourcesCount
};

class ResourceBase : public std::enable_shared_from_this<ResourceBase>
{
protected:
	ResourceType resourceType = ResourceType::Unknown;

	std::wstring name;
	std::wstring path;
	UINT id = 0;

protected:
	virtual bool Save(const std::wstring& path) { return false; };
	virtual bool Load(const std::wstring& path) { return false; };

public:
	ResourceBase(ResourceType type);
	virtual ~ResourceBase();

	ResourceType& GetType() { return resourceType; };

	void SetName(const std::wstring& name) { this->name = name; };
	std::wstring& GetName() { return name; };
	UINT& GetID() { return id; };
};

