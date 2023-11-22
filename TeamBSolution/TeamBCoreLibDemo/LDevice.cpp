#include "LDevice.h"

bool LDevice::SetDevice()
{
    UINT Flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
    HRESULT hr = D3D11CreateDevice(
        NULL,
        m_DriverType,
        NULL,
        Flags,
        &m_FeatureLevels, 1,
        D3D11_SDK_VERSION,
        &m_pDevice,
        NULL,
        &m_pImmediateContext
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
        __uuidof(IDXGIFactory), (LPVOID*)&m_pGIFactory
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
    
    SwapChainDesc.BufferDesc.Width = 800;
    SwapChainDesc.BufferDesc.Height = 600;
    SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    SwapChainDesc.SampleDesc.Count = 1;
    SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    SwapChainDesc.BufferCount = 1;
    SwapChainDesc.OutputWindow = m_hWnd;
    SwapChainDesc.Windowed = true;

    HRESULT hr = m_pGIFactory->CreateSwapChain(m_pDevice, &SwapChainDesc, &m_pSwapChain);

    if (FAILED(hr))
    {
        MessageBoxA(NULL, "Create SwapChain Error", "Error Box", MB_OK);
        m_pGIFactory->Release();
        return false;
    }

    m_pGIFactory->Release();
    return true;
}

bool LDevice::SetDepthTexture()
{
    DXGI_SWAP_CHAIN_DESC scDesc;
    m_pSwapChain->GetDesc(&scDesc);

    D3D11_TEXTURE2D_DESC texDesc = { 0, };
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
    D3D11_DEPTH_STENCIL_VIEW_DESC dsDesc = {};
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
    HRESULT hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&m_pbackBuffer);

    if (FAILED(hr))
    {
        MessageBoxA(NULL, "SwapChain not get Buffer", "Error Box", MB_OK);
        return false;
    }

    hr = m_pDevice->CreateRenderTargetView(m_pbackBuffer, NULL, &m_pRenderTargetView);

    if (FAILED(hr))
    {
        MessageBoxA(NULL, "Create Render TargetView Error", "Error Box", MB_OK);
        m_pbackBuffer->Release();
        return false;
    }

    m_pbackBuffer->Release();

    return true;
}

bool LDevice::SetViewPort()
{
    // viewPort
    m_ViewPort.TopLeftX = 0.0f;
    m_ViewPort.TopLeftY = 0.0f;
    m_ViewPort.Width = 800.0f;
    m_ViewPort.Height = 600.0f;
    m_ViewPort.MinDepth = 0.0f;
    m_ViewPort.MaxDepth = 1.0f;
 
    return true;
}

bool LDevice::DeviceInit()
{
    // ��ǻ� create �Լ� �̸��� ���ļ� Set���� ����
    SetDevice();
    SetGIFactory();
    SetSwapChain();
    // ���� �ؽ��� ����
    SetDepthTexture();
    // ���� �ؽ��� ��� ���ٽ� �� ����
    SetDepthStencilView();
    SetRenderTargetView();
    SetViewPort();
    return true;
}

bool LDevice::DeviceFrame()
{
    return true;
}

bool LDevice::DevicePreRender()
{
    float color[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
    m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, color);
    // ���ٽ� �� �ʱ�ȭ ���ĽǺ並 1.0f�� �ʱ�ȭ �ϰ� ���Ŀ� �׷����°͵鿡�Դ� 1.0f���� ���������� �ش�
    m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(),
        D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    // 1 ������ RenderTargetView���� 2 RenderTargetView�迭 3 ���ĽǺ�
     // RenderTarget �ȼ��� ������ �����ϴ°�
    // ������ �ܰ迡�� �� ���ۿ� �׷��� �ȼ����� ������ �ȼ��� ���̵� ���⼭ �����ϴ� �����ε�
    m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView.Get());
    // 1 ����Ʈ�� ����, 2����Ʈ �迭
    m_pImmediateContext->RSSetViewports(1, &m_ViewPort);
    
    return true;
}

bool LDevice::DevicePostRender()
{
    HRESULT hr = m_pSwapChain->Present(0, 0);
    if (FAILED(hr))
    {
        MessageBoxA(NULL, "buffer Present Error", "Error Box", MB_OK);
        return false;
    }

    return true;
}

bool LDevice::DeviceRelease()
{
    if (m_pSwapChain) m_pSwapChain->Release();
    if (m_pDevice) m_pDevice->Release();
    if (m_pImmediateContext) m_pImmediateContext->Release();
    if (m_pRenderTargetView) m_pRenderTargetView->Release();
    return true;
}

LDevice::~LDevice() {}
