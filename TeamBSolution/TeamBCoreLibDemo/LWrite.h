#pragma once
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <string>
#include <vector>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

struct LTextData
{
	std::wstring text = L"";
	D2D1_RECT_F layout = { 0.0f, 0.0f, 800.0f, 600.0f };
	D2D1::ColorF color = { 0.0, 0.0f, 0.0f, 1.0f };
};

class LWrite
{
public:
	static LWrite& GetInstance()
	{
		static LWrite input;
		return input;
	}
public:
	std::vector<LTextData> m_TextList;
public:
	ID2D1Factory* m_pFactory = nullptr;
	IDWriteFactory* m_pWriteFactory = nullptr;
	IDWriteTextFormat* m_ptextFormat = nullptr;
	IDXGISwapChain* m_pSwapChain = nullptr;
	IDXGISurface1* m_pbackBuffer = nullptr;
	ID2D1RenderTarget* m_pRenderTarget = nullptr;
	ID2D1SolidColorBrush* m_pSolidColorBrush = nullptr;
public:
	void SetCreateFactory();
	void SetCreateWriteFactory();
	void SetCreateTextFormat();
	void SetCreateBackBuffer(IDXGISwapChain* pSwapChain);
	void SetCreateWriteRT(HWND hWnd);
public:
	void AddText(std::wstring text, float x, float y, D2D1::ColorF color = { 0.0f, 0.0f, 0.0f, 1.0f });
public:
	bool Create(IDXGISwapChain* pSwapChain, HWND hWnd);
	bool Init();
	bool Frame();
	bool PreRender();
	bool Render();
	bool PostRender();
	bool Release();
};

