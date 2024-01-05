#include "LDevice.h"
#include "LGlobal.h"
bool LDevice::SetDevice()
{
    UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#ifdef _DEBUG
    flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_HARDWARE;
    D3D_FEATURE_LEVEL featureLevels = D3D_FEATURE_LEVEL_11_0;
    
    HRESULT hr = D3D11CreateDevice(
        NULL,
        driverType,
        NULL,
        flags,
        &featureLevels, 1,
        D3D11_SDK_VERSION,
        m_pDevice.GetAddressOf(),
        NULL,
        m_pImmediateContext.GetAddressOf()
    );

    if (FAILED(hr))
    {
        MessageBoxA(NULL, "Create Device Error", "Error Box", MB_OK);
        return false;
    }

    return true;
}

bool LDevice::SetGIFactory()
{
    HRESULT hr = CreateDXGIFactory(
        __uuidof(IDXGIFactory), (LPVOID*)m_pGIFactory.GetAddressOf()
    );

    if (FAILED(hr))
    {
        MessageBoxA(NULL, "Create DXGIFactory Error", "Error Box", MB_OK);
        return false;
    }

    return true;
}

bool LDevice::SetSwapChain()
{
    DXGI_SWAP_CHAIN_DESC SwapChainDesc = {0,};
    
    SwapChainDesc.BufferDesc.Width = LGlobal::g_WindowWidth;
    SwapChainDesc.BufferDesc.Height = LGlobal::g_WindowHeight;
    SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    SwapChainDesc.SampleDesc.Count = 1;
    SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    SwapChainDesc.BufferCount = 1;
    SwapChainDesc.OutputWindow = m_hWnd;
    SwapChainDesc.Windowed = true;

    HRESULT hr = m_pGIFactory->CreateSwapChain(m_pDevice.Get(), &SwapChainDesc, m_pSwapChain.GetAddressOf());

    if (FAILED(hr))
    {
        MessageBoxA(NULL, "Create SwapChain Error", "Error Box", MB_OK);
        return false;
    }

    return true;
}

bool LDevice::SetDepthTexture()
{
    DXGI_SWAP_CHAIN_DESC scDesc;
    m_pSwapChain->GetDesc(&scDesc);

    D3D11_TEXTURE2D_DESC texDesc;
    ZeroMemory(&texDesc, sizeof(texDesc));
    texDesc.Width = scDesc.BufferDesc.Width;
    texDesc.Height = scDesc.BufferDesc.Height;
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 1;
    texDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
    texDesc.SampleDesc = { 1, 0 };
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    texDesc.CPUAccessFlags = 0;
    texDesc.MiscFlags = 0;

    HRESULT hr = m_pDevice->CreateTexture2D(&texDesc, NULL, m_DepthTexture.GetAddressOf());

    if (FAILED(hr))
    {
        MessageBoxA(NULL, "Create Stencil 2DTexture Error", "Error Box", MB_OK);
        return false;
    }

    return true;
}

bool LDevice::SetDepthStencilView()
{
    D3D11_DEPTH_STENCIL_VIEW_DESC dsDesc;
    ZeroMemory(&dsDesc, sizeof(dsDesc));
    dsDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

    HRESULT hr = m_pDevice->CreateDepthStencilView(m_DepthTexture.Get(),
        &dsDesc, m_pDepthStencilView.GetAddressOf());

    if (FAILED(hr))
    {
        MessageBoxA(NULL, "Create StencilView Error", "Error Box", MB_OK);
        return false;
    }

    return true;
}

bool LDevice::SetRenderTargetView()
{
    HRESULT hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)m_pbackBuffer.GetAddressOf());

    if (FAILED(hr))
    {
        MessageBoxA(NULL, "SwapChain not get Buffer", "Error Box", MB_OK);
        return false;
    }

    hr = m_pDevice->CreateRenderTargetView(m_pbackBuffer.Get(), NULL, m_pRenderTargetView.GetAddressOf());

    if (FAILED(hr))
    {
        MessageBoxA(NULL, "Create Render TargetView Error", "Error Box", MB_OK);
        return false;
    }

    return true;
}

bool LDevice::SetViewPort()
{
    // viewPort
    ZeroMemory(&m_ViewPort, sizeof(m_ViewPort));
  
    m_ViewPort.TopLeftX = 0.0f;
    m_ViewPort.TopLeftY = 0.0f;
    m_ViewPort.Width = LGlobal::g_WindowWidth;
    m_ViewPort.Height = LGlobal::g_WindowHeight;
    m_ViewPort.MinDepth = 0.0f;
    m_ViewPort.MaxDepth = 1.0f;
 
    return true;
}

bool LDevice::Init()
{
    // Create 함수 이름이 겹쳐서 Set 생성
    SetDevice();
    SetGIFactory();
    SetSwapChain();
    // 깊이 텍스쳐 생성
    SetDepthTexture();
    // 깊이 텍스쳐 기반 스텐실 뷰 생성
    SetDepthStencilView();
    SetRenderTargetView();
    SetViewPort();
    return true;
}

bool LDevice::Frame()
{
    return true;
}

bool LDevice::PreRender()
{
    float color[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
    m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView.Get(), color);
    // 스텐실 뷰 초기화 스탠실뷰를 1.0f로 초기화 하고 이후에 그려지는것들에게는 1.0f보다 작은값들을 준다
    m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(),
        D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    // 1 설정할 RenderTargetView갯수 2 RenderTargetView배열 3 스탠실뷰
     // RenderTarget 픽셀의 색상값을 저장하는곳
    // 마지막 단계에서 백 버퍼에 그려질 픽셀값을 결정함 픽셀의 깊이도 여기서 조절하는 이유인듯
    m_pImmediateContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
    // 1 뷰포트의 갯수, 2뷰포트 배열
    m_pImmediateContext->RSSetViewports(1, &m_ViewPort);
    
    return true;
}

bool LDevice::PostRender()
{
    HRESULT hr = m_pSwapChain->Present(0, 0);

    if (FAILED(hr))
    {
        MessageBoxA(NULL, "buffer Present Error", "Error Box", MB_OK);
        return false;
    }

    return true;
}

bool LDevice::Release()
{
    return true;
}

LDevice::~LDevice() {}
