#pragma once

// Windows Header.
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// C++ RunTime Header.
#include <string>
#include <bitset>

// D2D1 Runtimer header.
#include <dxgi1_4.h>
#include <D3D11.h>
#include <d2d1_3.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d2d1.lib" )
#pragma comment(lib, "dxguid.lib")

// Smart Pointer Header.
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;



struct HPAINTRESULT
{
	const unsigned char* bits;
	UINT width;
	UINT height;
};

enum : WPARAM
{
	VK_A = 0x41, VK_B = 0x42, VK_C = 0x43, VK_D = 0x44,
	VK_E = 0x45, VK_F = 0x46, VK_G = 0x47, VK_H = 0x48,
	VK_I = 0x49, VK_J = 0x4A, VK_K = 0x4B, VK_L = 0x4C,
	VK_M = 0x4D, VK_N = 0x4E, VK_O = 0x4F, VK_P = 0x50,
	VK_Q = 0x51, VK_R = 0x52, VK_S = 0x53, VK_T = 0x54,
	VK_U = 0x55, VK_V = 0x56, VK_W = 0x57, VK_X = 0x58,
	VK_Y = 0x59, VK_Z = 0x5A
};

struct KeyEvents
{
private:
	static const unsigned int size = 256;
	std::bitset<size> data;

public:
	void Add(WPARAM key) { if (!data[key % size]) { data[key % size] = 1; } }
	void Remove(WPARAM key) { if (data[key % size]) { data[key % size] = 0; } }
	bool Match(WPARAM key) { return data[key % size]; }
	bool IsValid() { return data.count() > 0; }
	void Clear() { data.reset(); }
};

/**
 * @brief D2DApp template.
 */
class D2DApp
{
	struct D2DAppData
	{
		friend class D2DApp;
	private:
		D2D1_RECT_U rect = { 0, 0, width, height };

	public:
		UINT width = 700;
		UINT height = 700;
		bool bLeftMousePressing = false;
		bool bMiddleMousePressing = false;
		bool bRightMousePressing = false;
		bool bResizing = false;
		bool bWindowMaximum = false;
		KeyEvents keys;
	};

public:
	D2DAppData appData;

	D2DApp();

	virtual ~D2DApp();

protected:
	/// Override message handle.
	virtual HRESULT HandleCreateEvent(UINT width, UINT height) { return S_OK; }

	virtual void HandleActivateEvent(WPARAM nFlags, HWND hwnd) {}

	virtual void HandleKeyDownEvent(WPARAM nKey) {}

	virtual void HandleKeyUpEvent(WPARAM nKey) {}
	
	virtual void HandleLeftMouseDownEvent(WPARAM nFlags, int x, int y) {}

	virtual void HandleLeftMouseUpEvent(WPARAM nFlags, int x, int y) {}

	virtual void HandleMiddleMouseDownEvent(WPARAM nFlags, int x, int y) {}

	virtual void HandleMiddleMouseUpEvent(WPARAM nFlags, int x, int y) {}

	virtual void HandleRightMouseDownEvent(WPARAM nFlags, int x, int y) {}

	virtual void HandleRightMouseUpEvent(WPARAM nFlags, int x, int y) {}

	virtual void HandleMouseMoveEvent(WPARAM nFlags, int x, int y) {}

	virtual void HandleMouseWheelEvent(UINT nFlags, short zDelta, int x, int y) {}

	virtual void HandleResizeEvent(UINT width, UINT height) {}

	virtual void HandleDestroyEvent() {}

	virtual HPAINTRESULT HandlePaintEvent(const float deltaTime) = 0;

	virtual void Tick(const float deltaTime) = 0;
	/// Override message handle.


private:
	// Initialize device-independent resources.
	HRESULT CreateDeviceIndependentResources();

	// Initialize device-dependent resources.
	HRESULT CreateDeviceResources();

	// Device-dependent resources when window resize.
	void CreateWindowSizeDependentResources();

	// Update content.
	void UpdateScene(const float& deltaTime);

	// Calculate frames info.
	void CalculateFrameStats();

	// Interanl tick.
	void OnTick(const float& deltaTime);
	
	
	/// Internal message handle.
	HRESULT OnCreate();

	LRESULT OnActivate(const WPARAM& nFlags, const HWND& hwnd);

	LRESULT OnKeyDown(const WPARAM& nKey);

	LRESULT OnKeyUp(const WPARAM& nKey);
	
	LRESULT OnLeftMouseDown(const WPARAM& nFlags, const int& x, const int& y);

	LRESULT OnLeftMouseUp(const WPARAM& nFlags, const int& x, const int& y);

	LRESULT OnMiddleMouseDown(const WPARAM& nFlags, const int& x, const int& y);

	LRESULT OnMiddleMouseUp(const WPARAM& nFlags, const int& x, const int& y);

	LRESULT OnRightMouseDown(const WPARAM& nFlags, const int& x, const int& y);

	LRESULT OnRightMouseUp(const WPARAM& nFlags, const int& x, const int& y);

	LRESULT OnMouseMove(const WPARAM& nFlags, const int& x, const int& y);

	LRESULT OnMouseWheel(const UINT& nFlags, const short& zDelta, const int& x, const int& y);

	LRESULT OnSizing(const WPARAM& nFlags);

	LRESULT OnSize(const WPARAM& nFlags, const UINT& width, const UINT& height);

	LRESULT OnExitSizeMove();

	LRESULT OnDestroy();
	/// Internal message handle.


public:
	// Register the window classand call methods for instantiating drawing resources.
	HRESULT Initialize(HINSTANCE hInstance, int nCmdShow);

	// Process and dispatch messages.
	void Run();

	// Processes messages for the main window.
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


protected:

	HINSTANCE                   m_hAppInst = nullptr;

	HWND                        m_hWnd = nullptr;

	const char* const           m_wndCaption = "D2DApp";


private:
	/// D2D
	ComPtr<ID3D11Device>        m_pD3DDevice;

	ComPtr<ID3D11DeviceContext> m_pD3DDeviceContext;

	ComPtr<ID2D1Factory4>       m_pD2DFactory;

	ComPtr<ID2D1Device3>        m_pD2DDevice;

	ComPtr<ID2D1DeviceContext3> m_pD2DDeviceContext;

	ComPtr<ID2D1Bitmap1>        m_pD2DTargetBimtap;

	ComPtr<ID2D1Bitmap1>        m_pD2DRenderTarget;

	ComPtr<IDXGISwapChain1>     m_pSwapChain;

	D3D_FEATURE_LEVEL           m_featureLevel;

	DXGI_PRESENT_PARAMETERS     m_parameters;
	/// D2D
};