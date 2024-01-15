#include "TextureManager.h"
static std::wstring to_mw(const std::string& _src)
{
    USES_CONVERSION;
    return std::wstring(A2W(_src.c_str()));
};
#define _tsplitpath_s   _wsplitpath_s


W_STR TextureManager::GetSplitName(std::wstring fullpath)
{
    W_STR name;
    TCHAR dirve[MAX_PATH] = { 0, };
    TCHAR dir[MAX_PATH] = { 0, };
    TCHAR filename[MAX_PATH] = { 0, };
    TCHAR ext[MAX_PATH] = { 0, };
    _tsplitpath_s(fullpath.c_str(),
        dirve, dir, filename, ext);
    name = filename;
    name += ext;
    return name;
}
W_STR TextureManager::GetSplitName(std::string fullpath)
{
    W_STR szUnicode = to_mw(fullpath);
    TCHAR dirve[MAX_PATH] = { 0, };
    TCHAR dir[MAX_PATH] = { 0, };
    TCHAR filename[MAX_PATH] = { 0, };
    TCHAR ext[MAX_PATH] = { 0, };
    _tsplitpath_s(szUnicode.c_str(), dirve, dir, filename, ext);
    W_STR fileName = filename;
    W_STR fileExt = ext;
    if (fileExt == L".tga" || fileExt == L".TGA")
    {
        fileExt = L".dds";
    }
    fileName += fileExt;
    return fileName;
}
void TextureManager::SetDevice(
    ID3D11Device* pd3dDevice,
    ID3D11DeviceContext* pContext)
{
    m_pd3dDevice = pd3dDevice;
    m_pImmediateContext = pContext;
}
Texture* TextureManager::Load(std::wstring name)
{
    HRESULT hr;
    // 중복제거
    auto data = Find(name);
    if (data != nullptr) return data;

    Texture* pNewData = new Texture;
    if (pNewData)
    {
        hr = pNewData->Load(m_pd3dDevice, m_pImmediateContext, name);
        if (SUCCEEDED(hr))
        {
            m_List.insert(std::make_pair(name, pNewData));
            return pNewData;
        }
    }
    delete pNewData;
    return nullptr;
}
Texture* TextureManager::Find(std::wstring name)
{
    auto iter = m_List.find(name);
    if (iter != m_List.end())
    {
        return iter->second;
    }
    return nullptr;
}
TextureManager::TextureManager()
{
}
bool TextureManager::Release()
{
    for (auto data : m_List)
    {
        Texture* pTexture = data.second;
        if (pTexture) pTexture->Release();
        delete pTexture;
    }
    m_List.clear();
    return true;
}
TextureManager::~TextureManager()
{
    Release();
}