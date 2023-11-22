#include "LFbxMgr.h"

LFbxObj* LFbxMgr::Load(std::wstring filePath)
{
	std::size_t found = filePath.find_last_of(L"/");
	std::wstring path = filePath.substr(0, found + 1);
	std::wstring key = filePath.substr(found + 1);

	LFbxObj* data = GetPtr(key);

	if (data != nullptr)
	{
		return data;
	}

	LFbxObj* ret = nullptr;

	std::shared_ptr<LFbxObj> managerType = std::make_shared<LFbxObj>();
	m_FbxImport.Init();
	if (m_FbxImport.Load(filePath, managerType.get()))
	{
		m_map.insert(std::make_pair(key, managerType));
		ret = managerType.get();
	}

	m_FbxImport.Release();

	return ret;
}

LFbxObj* LFbxMgr::GetPtr(std::wstring key)
{
	auto iter = m_map.find(key);

	if (iter == m_map.end())
	{
		return nullptr;
	}

	return iter->second.get();
}