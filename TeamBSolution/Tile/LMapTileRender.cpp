#include "LMapTileRender.h"

bool LMapTileRender::Build(LMap* pMap, DWORD dwWidth, DWORD dwHeight)
{
    CreateAlphaTexture(m_pDevice.Get(), 1024, 1024);
    _dwWidth = dwWidth;
    _dwHeight = dwHeight;
    _pMap = pMap;
    _iNumFace = (dwWidth - 1) * (dwHeight - 1) * 2;
    _bUsedIndexList = true;
    
    m_RootNode = CreateNode(NULL, 0, dwWidth - 1, dwWidth * (dwHeight - 1), dwWidth * dwHeight - 1);
    

    return false;
}

LNode* LMapTileRender::CreateNode(LNode* pParentNode, DWORD dwTL, DWORD dwTR, DWORD dwBL, DWORD dwBR)
{
    return nullptr;
}

TVector2 LMapTileRender::GetHeightFromNode(DWORD dwTL, DWORD dwTR, DWORD dwBL, DWORD dwBR)
{
    return TVector2();
}

void LMapTileRender::ComputeBoundingBox(LNode* pNode)
{
}

void LMapTileRender::UpdateBoundingBox(LNode* pNode)
{
}

bool LMapTileRender::SubDivide(LNode* pNode)
{
    return false;
}

bool LMapTileRender::Release()
{
    return false;
}

bool LMapTileRender::Render(ID3D11DeviceContext* pContext)
{
    return false;
}

bool LMapTileRender::CreateVertexList(LNode* pNode)
{
    return false;
}

bool LMapTileRender::CreateIndexList(LNode* pNode)
{
    return false;
}

HRESULT LMapTileRender::CreateAlphaTexture(ID3D11Device* pDevice, DWORD dwWidth, DWORD dwHeight)
{
    HRESULT hr = S_OK;
    /* The dimension value becomes a trade - off between quality & storage requirements
    * 품질과 저장공간요구 사이는 상쇄관계에 있다. 고객만족을 위해 품질이 증가하려면 반드시 저장공간이 증가해아함
    */
    const UINT LOOKUP_DIMENSION = 512;
    
    // Describe the texture. 텍스쳐에 대한 설명
    D3D11_TEXTURE2D_DESC desc;
    desc.ArraySize = 1;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.Height = dwHeight;
    desc.Width = dwWidth;
    desc.MipLevels = 1;
    desc.MiscFlags = 0;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;

    // Generate the initial data
    _fLookup = new BYTE[dwWidth * dwHeight * 4];

    for (UINT x = 0; x < dwHeight; ++x)
    {
        for (UINT y = 0; y < dwWidth; ++y)
        {
            BYTE* pixel = &_fLookup[y * 4 + x * dwWidth * 4];
            pixel[0] = 255;
            pixel[1] = 255;
            pixel[2] = 0;
            pixel[3] = 255;
        }
    }

    D3D11_SUBRESOURCE_DATA initData;
    initData.pSysMem = _fLookup;
    initData.SysMemPitch = sizeof(BYTE) * 4 * dwWidth;
    initData.SysMemSlicePitch = 0;

    hr = pDevice->CreateTexture2D(&desc, &initData, _pRoughnessLookUpTex.GetAddressOf());
    if (FAILED(hr))
    {
        SAFE_DELETE_ARRAY(_fLookup);
        return hr;
    }

    hr = pDevice->CreateShaderResourceView(_pRoughnessLookUpTex.Get(), NULL, _pLookupSRV.GetAddressOf());
    if (FAILED(hr))
    {
        SAFE_DELETE_ARRAY(_fLookup);
        return hr;
    }
    
    UINT const dataSize = sizeof(BYTE) * 4;
    UINT const rowPitch = dataSize * dwWidth;
    UINT const depthPitch = 0; // dataSize * dwWidth * dwHeight;
    for (UINT x = 0; x < dwHeight; ++x)
    {
        for (UINT y = 0; y < dwWidth; ++y)
        {
            BYTE* pixel = &_fLookup[y * 4 + x * dwWidth * 4];
            pixel[0] = 0;
            pixel[1] = 0;
            pixel[2] = 255;
            pixel[3] = 255;
        }
    }
    D3D11_BOX box;
    box.left = 0;
    box.right = dwWidth;
    box.top = 0;
    box.bottom = dwHeight;
    box.front = 0;
    box.back = 1;

    m_pImmediateContext->UpdateSubresource(_pRoughnessLookUpTex.Get(), 0, &box, _fLookup, rowPitch, depthPitch);
    

    return hr;
}

LMapTileRender::LMapTileRender(void)
{
}

LMapTileRender::~LMapTileRender(void)
{
}
