#pragma once

// Windows Header.
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

// D2D1 Runtimer header.
#include <dxgi1_4.h>
#include <D3D11.h>
#include <d2d1_3.h>

// Smart Pointer Header.
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;



/**
 * @brief D2DApp template.
 */
class D2DApp
{
public:

	D2DApp();

	virtual ~D2DApp();

protected:
	//~ Begin override message handle.
	
	virtual bool HandleCreateEvent(UINT width, UINT height) { return false; }

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

	virtual void Tick(const float deltaTime) = 0;
	
	//~ End override message handle.


	bool IsResizing() const noexcept { return m_bResizing; }

	bool IsWindowMaximum() const noexcept { return m_bWindowMaximum; }

	void Draw(
		const unsigned char* bits,
		unsigned int width,
		unsigned int height,
		unsigned int strides
	);

private:
	// Initialize device-independent resources.
	HRESULT CreateDeviceIndependentResources();

	// Initialize device-dependent resources.
	HRESULT CreateDeviceResources();

	// Device-dependent resources when window resize.
	void CreateWindowSizeDependentResources();

	// Calculate frames info.
	void CalculateFrameStats();

	// Interanl tick.
	void OnTick(const float& deltaTime);
	
	

	//~ Begin internal message handle.
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
	//~ End internal message handle.


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

	UINT                        m_width = 700;
	
	UINT                        m_height = 700;

private:
	//~ Begin D2D
	D2D1_RECT_U                 m_rect = { 0, 0, m_width, m_height };

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
	//~ End D2D


	//~ Begin operation.
	bool                        m_bResizing = false;
	
	bool                        m_bWindowMaximum = false;
	//~ End operation.
};