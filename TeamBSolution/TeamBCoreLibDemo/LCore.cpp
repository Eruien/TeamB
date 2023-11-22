#include "LCore.h"
#include "LGlobal.h"
#include "LInput.h"

bool LCore::Init() { return true; }
bool LCore::Frame() { return true; }
bool LCore::Render() { return true; }
bool LCore::Release() { return true; }

void LCore::CreateSamplerState()
{
    D3D11_SAMPLER_DESC descSampler;
    descSampler.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

    descSampler.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    descSampler.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    descSampler.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

    descSampler.MipLODBias = 0;
    descSampler.MaxAnisotropy = 16;

    descSampler.ComparisonFunc = D3D11_COMPARISON_NEVER;

    descSampler.BorderColor[0] = 1.0f;
    descSampler.BorderColor[1] = 0.0f;
    descSampler.BorderColor[2] = 0.0f;
    descSampler.BorderColor[3] = 1.0f;

    descSampler.MinLOD = 0;
    descSampler.MaxLOD = D3D11_FLOAT32_MAX;
    m_pDevice->CreateSamplerState(&descSampler, m_pSamplerState.GetAddressOf());
}

void LCore::CreateDepthStencilState()
{
    HRESULT hr;
    // ���̹��� ���°� ����
    D3D11_DEPTH_STENCIL_DESC dsDescDepth = {0,};
    dsDescDepth.DepthEnable = TRUE;
    // ���δ� �׸���
    dsDescDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    // �ش� �ȼ��� ���� ������ �Ͽ� ��¿��θ� �Ǵ��Ѵ�.
    // ������ ���� �� >=  ���̹����� �ȼ���(1.0f)
    // �ʱ�ȭ �ϴ� ���� ���� ������ �׷�����
    dsDescDepth.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

    //Stencil
    dsDescDepth.StencilEnable = FALSE;
    dsDescDepth.StencilReadMask = 1;
    dsDescDepth.StencilWriteMask = 1;
    dsDescDepth.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    dsDescDepth.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
    dsDescDepth.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsDescDepth.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    // ����Ʈ ��
    dsDescDepth.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    dsDescDepth.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsDescDepth.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsDescDepth.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

    // ���ʿ�  dsDescDepth.DepthEnable�� true�� ����
    if (FAILED(hr = m_pDevice->CreateDepthStencilState(&dsDescDepth,
        m_pDepthStencilState.GetAddressOf())))
    {
        return;
    }

    dsDescDepth.DepthEnable = FALSE;

    // ���ʿ�  dsDescDepth.DepthEnable�� false�� ����
    if (FAILED(hr = m_pDevice->CreateDepthStencilState(&dsDescDepth,
        m_pDepthStencilStateDisable.GetAddressOf())))
    {
        return;
    }

    return;
}

void LCore::CreateRasterizerState()
{
    HRESULT hr;
    D3D11_RASTERIZER_DESC rd;
    ZeroMemory(&rd, sizeof(rd));
    rd.CullMode = D3D11_CULL_NONE;
    rd.FillMode = D3D11_FILL_SOLID;
    hr = m_pDevice->CreateRasterizerState(&rd, m_pRSSolid.GetAddressOf());
    rd.FillMode = D3D11_FILL_WIREFRAME;
    hr = m_pDevice->CreateRasterizerState(&rd, m_pRSWireFrame.GetAddressOf());
}

bool LCore::EngineInit()
{
    DeviceInit();

    LGlobal::g_pDevice = m_pDevice;
    LGlobal::g_pImmediateContext = m_pImmediateContext;

    CreateSamplerState();
    CreateDepthStencilState();
    CreateRasterizerState();

    LManager<LTexture>::GetInstance().Set(m_pDevice, m_pImmediateContext);
    LManager<LShader>::GetInstance().Set(m_pDevice, m_pImmediateContext);
    m_Gametimer.Init();
    m_pDefaultCamera = std::make_shared<LCamera>();
    m_pDefaultCamera->Init();
    m_pDefaultCamera->CreateLookAt({ 0.0f, 600.0f, -300.0f }, { 0.0f, 0.0f, 1.0f });
    m_pDefaultCamera->CreatePerspectiveFov(L_PI * 0.25, (float)g_WindowWidth / (float)g_WindowHeight, 1.0f, 10000.0f);
    LGlobal::g_pMainCamera = m_pDefaultCamera.get();
    LInput::GetInstance().Init();
    LWrite::GetInstance().Init();
    LWrite::GetInstance().Create(m_pSwapChain, m_hWnd);
	Init();
	return true;
}

bool LCore::EngineFrame()
{
    DeviceFrame();
    m_Gametimer.Frame();
    LInput::GetInstance().Frame();
    LWrite::GetInstance().Frame();
    LGlobal::g_pMainCamera->Frame();
	Frame();
	return true;
}

bool LCore::EngineRender()
{
    DevicePreRender();
    m_pImmediateContext->PSSetSamplers(0, 1, m_pSamplerState.GetAddressOf());
    // ���Ľ� ������Ʈ �ʿ� 1�� lessequal�� �����߱� ������ 1���� ���ų� ������ �׷����� �ߴ�
    m_pImmediateContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), 1);

    if (LInput::GetInstance().m_dwKeyState[VK_F1] == DWORD(KeyState::KEY_PUSH))
    {
        m_ISWireFrame = !m_ISWireFrame;
    }
    m_pImmediateContext->RSSetState(m_pRSWireFrame.Get());
    if (m_ISWireFrame)
    {
        m_pImmediateContext->RSSetState(m_pRSWireFrame.Get());
    }
    else
    {
        m_pImmediateContext->RSSetState(m_pRSSolid.Get());
    }
    LWrite::GetInstance().PreRender();
	Render();
    LGlobal::g_pMainCamera->Render();
    m_Gametimer.Render();
    LWrite::GetInstance().Render();
    LInput::GetInstance().Render();
    LWrite::GetInstance().PostRender();
    DevicePostRender();
	return true;
}

bool LCore::EngineRelease()
{
    DeviceRelease();
    Release();
    m_Gametimer.Release();
    LWrite::GetInstance().Release();
    LGlobal::g_pMainCamera->Release();
    LInput::GetInstance().Release();
    LManager<LTexture>::GetInstance().Release();
    LManager<LShader>::GetInstance().Release();
    
	return true;
}

bool LCore::Run()
{
    EngineInit();

    MSG msg = { 0 };

    while (msg.message != WM_QUIT)
    {
        if (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            EngineFrame();
            EngineRender();
        }
    }

    EngineRelease();

    return true;
}

LCore::~LCore() {}