#include "Sample.h"
#include "LGlobal.h"

bool Sample::Init()
{
	m_DebugCamera = std::make_shared<LDebugCamera>();
	m_DebugCamera->CreateLookAt({ 0.0f, 700.0f, -500.0f }, { 0.0f, 0.0f, 1.0f });
	m_DebugCamera->CreatePerspectiveFov(L_PI * 0.25, (float)LGlobal::g_WindowWidth / (float)LGlobal::g_WindowHeight, 1.0f, 10000.0f); // SetProjMatrix
	LGlobal::g_pMainCamera = m_DebugCamera.get();

	m_HeightMap = new LHeightMap;
	m_HeightMap->Set();
	m_HeightMap->CreateHeightMap(L"../../res/Heightmap/heightMap513.bmp");

	// �ؽ�ó�� ������� �� ũ�⸦ ���ϸ� �� ũ�⸸ŭ ���� ���������.
	// ũ��� 2��n�� +1 ũ�⸸ �����ϴ�.
	// ex 513, 1025,
	LMapDesc MapDesc = {};
	MapDesc.iNumCols = m_HeightMap->m_iNumCols;
	MapDesc.iNumRows = m_HeightMap->m_iNumRows;
	MapDesc.fCellDistance = 1.0f;
	MapDesc.fScaleHeight = 1.0f;
	MapDesc.ShaderFilePath = L"../../res/hlsl/DiffuseLight.hlsl";
	MapDesc.TextureFilePath = L"../../res/map/basecolor.jpg";
	m_HeightMap->Load(MapDesc);

	// �������� �ø��� Ʈ�� 
	m_Tree = new LQurdtree;
	m_Tree->Set();
	m_Tree->m_TreeDepth = 2;
	// �������� �ø��� ���̶� ���� ũ�⸦ �Է�
	m_Tree->BuildQurdTree(m_HeightMap, 513, 513);

	// light
	m_cbLight.g_cAmbientMaterial = TVector4(0.3f, 0.3f, 0.3f, 1);
	m_cbLight.g_cDiffuseMaterial = TVector4(1, 1, 1, 1);
	m_cbLight.g_cAmbientLightColor = TVector4(1, 1, 1, 1);
	m_cbLight.g_cDiffuseLightColor = TVector4(1, 1, 1, 1);

	// create constantbuffer

	m_pConstantBufferLight.Attach(CreateConstantBuffer(
							m_pDevice.Get(), &m_cbLight, 1, sizeof(LIGHT_CONSTANT_BUFFER) ));



	return true;
}

bool Sample::Frame()
{
	m_Tree->Frame();

	return true;
}

bool Sample::Render()
{
	m_Tree->Render();

	return true;
}

bool Sample::Release()
{
	return true;
}

ID3D11Buffer* Sample::CreateConstantBuffer(ID3D11Device* pd3dDevice, void* data, UINT iNumIndex, UINT iSize, bool bDynamic)
{
	HRESULT hr = S_OK;
	ID3D11Buffer* pBuffer = nullptr;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	if (bDynamic)
	{
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0;
	}
	bd.ByteWidth = iSize * iNumIndex;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	InitData.pSysMem = data;
	if (data != NULL)
	{
		if (FAILED(hr = pd3dDevice->CreateBuffer(&bd, &InitData, &pBuffer)))
		{
			//H(hr);
			return nullptr;
		}
	}
	else
	{
		if (FAILED(hr = pd3dDevice->CreateBuffer(&bd, NULL, &pBuffer)))
		{
			//H(hr);
			return nullptr;
		}
	}
	return pBuffer;
}

Sample::~Sample() {}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR IpCmdLine, int nCmdShow)
{
	Sample win;
	win.SetRegisterWindowClass(hInstance);
	win.SetCreateWindow(L"TeamBProject", 800, 600);
	win.Run();
}


