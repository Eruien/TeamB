#pragma once
#include "LStd.h"
#include "EnumClass.h"

class ResourceBase : public enable_shared_from_this<ResourceBase>
{
protected:
	ResourceType resourceType = ResourceType::Unknown;  // -1 ÃÊ±âÈ­

	wstring name;
	wstring path;
	UINT id = 0;
	
protected:
	virtual bool Save(const wstring& path) { return false; };
	virtual bool Load(const wstring& path) { return false; };

public:
	ResourceBase(ResourceType type);
	virtual ~ResourceBase();

	ResourceType& GetType() { return resourceType; };

	void SetName(const wstring& name) { this->name = name; };
	wstring& GetName() { return name; };
	UINT& GetID() { return id; };

};

