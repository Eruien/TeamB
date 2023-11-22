#include "Sample.h"

void Sample::CreateBlendState()
{
    D3D11_BLEND_DESC bsd = { 0, };
    bsd.RenderTarget[0].BlendEnable = true;
    bsd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    bsd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    bsd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

    bsd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    bsd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    bsd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    bsd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    m_pDevice->CreateBlendState(&bsd, &m_AlphaBlend);
}

bool Sample::Init()
{
    CreateBlendState();

    m_HeightMap = new LHeightMap;
    m_HeightMap->Set(m_pDevice, m_pImmediateContext);
    m_HeightMap->CreateHeightMap(L"../../res/Heightmap/heightMap513.bmp");
   
    LMapDesc MapDesc = {};
    MapDesc.iNumCols = m_HeightMap->m_iNumCols;
    MapDesc.iNumRows = m_HeightMap->m_iNumRows;
    MapDesc.fCellDistance = 1.0f;
    MapDesc.fScaleHeight = 1.0f;
    MapDesc.ShaderFilePath = L"../../res/hlsl/CustomizeMap.hlsl";
    MapDesc.TextureFilePath = L"../../res/map/basecolor.jpg";
    m_HeightMap->Load(MapDesc);

    m_CustomMap = new LMap;
    m_CustomMap->Set(m_pDevice, m_pImmediateContext);
    LMapDesc CMapDesc = {};
    CMapDesc.iNumCols = 513;
    CMapDesc.iNumRows = 513;
    CMapDesc.fCellDistance = 1.0f;
    CMapDesc.ShaderFilePath = L"../../res/hlsl/CustomizeMap.hlsl";
    CMapDesc.TextureFilePath = L"../../res/map/topdownmap.jpg";
    m_CustomMap->Load(CMapDesc);

    m_Tree = new LQurdtree;
    m_Tree->Set(m_pDevice, m_pImmediateContext);
    m_Tree->m_TreeDepth = 2;
    m_Tree->BuildQurdTree(m_HeightMap, 513, 513);

    m_DebugCamera = std::make_shared<LDebugCamera>();
    m_DebugCamera->CreateLookAt({ 0.0f, 200.0f, -100.0f }, { 0.0f, 0.0f, 1.0f });
    m_DebugCamera->CreatePerspectiveFov(L_PI * 0.25, (float)g_WindowWidth / (float)g_WindowHeight, 1.0f, 10000.0f);
    m_DebugCamera->Init();
    LGlobal::g_pMainCamera = m_DebugCamera.get();
    
    LFbxObj* fbxObj1 = LFbxMgr::GetInstance().Load(L"../../res/fbx/Turret_Deploy1/Turret_Deploy1.fbx");
    TurretModel = std::make_shared<LModel>();
    // 자식들에게 Device, Context전달
    TurretModel->Set(m_pDevice, m_pImmediateContext);
    TurretModel->SetLFbxObj(fbxObj1);
    TurretModel->SetChild();
    TurretModel->SetAnimationSpeed(30.0f);

    LFbxObj* fbxObj2 = LFbxMgr::GetInstance().Load(L"../../res/fbx/zombie/DeadMan_Mesh.fbx");
    ZombieModel = std::make_shared<LModel>();
    // 자식들에게 Device, Context전달
    ZombieModel->Set(m_pDevice, m_pImmediateContext);
    ZombieModel->SetLFbxObj(fbxObj2);
    ZombieModel->SetChild();
    ZombieModel->SetAnimationSpeed(30.0f);

    return true;
}

bool Sample::Frame()
{
    // 터렛모델 계산
    TurretModel->m_AnimationSpanTime += TurretModel->GetAnimationSpeed() * g_fSPF;
    
    if (TurretModel->m_AnimationSpanTime > TurretModel->GetLFbxObj()->m_MeshList[0]->m_iEndFrame)
    {
        TurretModel->m_AnimationSpanTime = 0.0f;
    }

    for (int iMesh = 0; iMesh < TurretModel->GetLFbxObj()->m_MeshList.size(); iMesh++)
    {
        memcpy(&TurretModel->m_SubModel[iMesh]->m_matWorld,
               &TurretModel->GetLFbxObj()->m_MeshList[iMesh]->m_MatrixArray[int(TurretModel->m_AnimationSpanTime)],
               sizeof(TMatrix));
    }

    // 좀비모델 계산
    ZombieModel->m_AnimationSpanTime += ZombieModel->GetAnimationSpeed() * g_fSPF;

    if (ZombieModel->m_AnimationSpanTime > ZombieModel->GetLFbxObj()->m_MeshList[0]->m_iEndFrame)
    {
        ZombieModel->m_AnimationSpanTime = 0.0f;
    }

    for (int iMesh = 0; iMesh < ZombieModel->GetLFbxObj()->m_MeshList.size(); iMesh++)
    {
        memcpy(&ZombieModel->m_SubModel[iMesh]->m_matWorld,
            &ZombieModel->GetLFbxObj()->m_MeshList[iMesh]->m_MatrixArray[int(ZombieModel->m_AnimationSpanTime)],
            sizeof(TMatrix));
    }

    // 높이맵 계산
    m_Tree->Frame();
  
    return true;
}

bool Sample::Render()
{
    m_pImmediateContext->OMSetBlendState(m_AlphaBlend, 0, -1);

    for (auto model : TurretModel->m_SubModel)
    {
        model->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
        model->Render();
    }

    // 주석 해제시 좀비 랜더링
  /*  for (auto model : ZombieModel->m_SubModel)
    {
        model->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
        model->Render();
    }*/

    // 주석 해제시 높이맵 랜더링
    //m_Tree->Render();
  
    // 주석 해제시 맵 랜더링
    //m_CustomMap->SetMatrix(nullptr, &LGlobal::g_pMainCamera->m_matView, &LGlobal::g_pMainCamera->m_matProj);
    //m_CustomMap->Render();
    return true;
}

bool Sample::Release()
{
    m_AlphaBlend->Release();

    return true;
}

Sample::~Sample() {}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR IpCmdLine, int nCmdShow)
{
    Sample win;
    win.SetRegisterWindowClass(hInstance);
    win.SetCreateWindow(L"창이다", 800, 600);
    win.Run();
}


