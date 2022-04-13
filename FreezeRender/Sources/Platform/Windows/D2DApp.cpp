#include "WindowsTargetVersion.h"

#include "D2DApp.hpp"
#include "Resource.h"

#include <Container/Array.hpp>

// C++ RunTime Header.
#include <sstream>
#include <Common.hpp>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d2d1.lib" )
#pragma comment(lib, "dxguid.lib")

#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))



/**
 * @brief time counter.
 */
class D2D1Timer
{
public:

	D2D1Timer()
	{
		m_secondsPerCount = 1.0 / static_cast<double>(GetPerformanceFrequency());
	}

	warn_nodiscard float TotalTime() const
	{
		return m_bStopped
			? static_cast<float>(((m_stopTime - m_pausedTime) - m_baseTime) * m_secondsPerCount)
			: static_cast<float>(((m_currTime - m_pausedTime) - m_baseTime) * m_secondsPerCount);
	}

	warn_nodiscard float GetDeltaTime() const
	{
		return static_cast<float>(m_deltaTime);
	}

	void Reset()
	{
		__int64 tempCurTime = GetPerformanceCounter();
		m_baseTime = tempCurTime;
		m_prevTime = tempCurTime;
		m_stopTime = 0;
		m_bStopped = false;
	}

	void Start()
	{
		if (m_bStopped)
		{
			__int64 tempCurTime = GetPerformanceCounter();
			m_pausedTime += (tempCurTime - m_stopTime);
			m_prevTime = tempCurTime;
			m_stopTime = 0;
			m_bStopped = false;
		}
	}

	void Stop()
	{
		if (!m_bStopped)
		{
			__int64 TempCurTime = GetPerformanceCounter();
			m_stopTime = TempCurTime;
			m_bStopped = true;
		}
	}

	void Tick()
	{
		if (m_bStopped)
		{
			m_deltaTime = 0.0;
			return;
		}

		__int64 tempCurTime = GetPerformanceCounter();
		m_currTime = tempCurTime;
		m_deltaTime = (m_currTime - m_prevTime) * m_secondsPerCount;
		m_prevTime = m_currTime;
		if (m_deltaTime < 0.0)
		{
			m_deltaTime = 0.0;
		}
	}

private:
	force_inline __int64 GetPerformanceFrequency() const
	{
		__int64 frequency;
		QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
		return frequency;
	}

	force_inline __int64 GetPerformanceCounter() const
	{
		__int64 counter;
		QueryPerformanceCounter((LARGE_INTEGER*)&counter);
		return counter;
	}

	bool	m_bStopped = false;

	double	m_secondsPerCount = 0.0;
	double	m_deltaTime = -1.0;

	__int64	m_baseTime = 0;
	__int64	m_pausedTime = 0;
	__int64	m_stopTime = 0;
	__int64	m_prevTime = 0;
	__int64	m_currTime = 0;
} g_timer;



////////////////////////////////////////////////////////////////
//
D2DApp::D2DApp()
{
	m_featureLevel = D3D_FEATURE_LEVEL_1_0_CORE; // just default value, not be used.
	m_parameters.DirtyRectsCount = 0;
	m_parameters.pDirtyRects = nullptr;
	m_parameters.pScrollRect = nullptr;
	m_parameters.pScrollOffset = nullptr;
}

D2DApp::~D2DApp()
{
}

void D2DApp::Draw(
	const unsigned char* bits,
	unsigned int width,
	unsigned int height,
	unsigned int strides
)
{
	if (m_pD2DDeviceContext && m_pD2DRenderTarget && !m_bWindowMinimum)
	{
		m_pD2DRenderTarget->CopyFromMemory(&m_rect, bits, strides);
		m_pD2DDeviceContext->BeginDraw();
		m_pD2DDeviceContext->DrawBitmap(m_pD2DRenderTarget.Get());
		m_pD2DDeviceContext->EndDraw();
		m_pSwapChain->Present(0, 0);
	}
}

HRESULT D2DApp::CreateDeviceIndependentResources()
{
	// Create a Direct2D factory.
	HRESULT hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		__uuidof(ID2D1Factory1),
		reinterpret_cast<void**>(m_pD2DFactory.GetAddressOf())
	);
	return hr;
}

HRESULT D2DApp::CreateDeviceResources()
{
	ComPtr<IDXGIDevice1> pDxgiDevice;
	HRESULT hr = S_OK;

	if (SUCCEEDED(hr))
	{
		UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_FEATURE_LEVEL featureLevels[] = {
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1
		};

		hr = D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			creationFlags,
			featureLevels,
			ARRAYSIZE(featureLevels),
			D3D11_SDK_VERSION,
			m_pD3DDevice.GetAddressOf(),
			&m_featureLevel,
			m_pD3DDeviceContext.GetAddressOf());
	}


	if (SUCCEEDED(hr))
	{
		hr = m_pD3DDevice->QueryInterface(IID_PPV_ARGS(pDxgiDevice.GetAddressOf()));
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pD2DFactory->CreateDevice(pDxgiDevice.Get(), m_pD2DDevice.GetAddressOf());
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pD2DDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, m_pD2DDeviceContext.GetAddressOf());
	}

	return hr;
}

void D2DApp::CreateWindowSizeDependentResources()
{
	ComPtr<IDXGIDevice1> pDxgiDevice;
	ComPtr<IDXGIAdapter> pDxgiAdapter;
	ComPtr<IDXGIFactory2> pDxgiFactory;
	ComPtr<IDXGISurface> pDxgiBackBuffer;

	HRESULT hr = S_OK;
	m_pD2DDeviceContext->SetTarget(nullptr);
	m_pD2DTargetBimtap.Reset();
	m_pD2DRenderTarget.Reset();
	m_pD3DDeviceContext->Flush();

	if (m_pSwapChain != nullptr)
	{
		hr = m_pSwapChain->ResizeBuffers(
			2, // Double-buffered swap chain.
			m_width,
			m_height,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			0);
		assert(hr == S_OK);
	}
	else
	{
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
		swapChainDesc.Width = m_width;
		swapChainDesc.Height = m_height;
		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.Stereo = false;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 2;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
		swapChainDesc.Flags = 0;
		swapChainDesc.Scaling = DXGI_SCALING_NONE;
		swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;


		if (SUCCEEDED(hr))
		{
			hr = m_pD3DDevice->QueryInterface(IID_PPV_ARGS(pDxgiDevice.GetAddressOf()));
		}

		if (SUCCEEDED(hr))
		{
			hr = pDxgiDevice->GetAdapter(pDxgiAdapter.GetAddressOf());
		}

		if (SUCCEEDED(hr))
		{
			hr = pDxgiAdapter->GetParent(IID_PPV_ARGS(pDxgiFactory.GetAddressOf()));
		}

		if (SUCCEEDED(hr))
		{
			hr = pDxgiFactory->CreateSwapChainForHwnd(
				m_pD3DDevice.Get(),
				m_hWnd,
				&swapChainDesc,
				nullptr,
				nullptr,
				m_pSwapChain.GetAddressOf());
		}

		if (SUCCEEDED(hr))
		{
			hr = pDxgiDevice->SetMaximumFrameLatency(1);
		}
	}


	if (SUCCEEDED(hr))
	{
		hr = m_pSwapChain->SetRotation(DXGI_MODE_ROTATION_IDENTITY);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(pDxgiBackBuffer.GetAddressOf()));
	}

	if (SUCCEEDED(hr))
	{
		D2D1_BITMAP_PROPERTIES1 properties = D2D1::BitmapProperties1(
			D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
			D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED));

		hr = m_pD2DDeviceContext->CreateBitmapFromDxgiSurface(
			pDxgiBackBuffer.Get(),
			&properties,
			m_pD2DTargetBimtap.GetAddressOf());
	}

	if (SUCCEEDED(hr))
	{
		m_pD2DDeviceContext->SetTarget(m_pD2DTargetBimtap.Get());
	}

	if (SUCCEEDED(hr))
	{
		m_pD2DDeviceContext->CreateBitmap(
			D2D1::SizeU(m_width, m_height),
			D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_IGNORE)),
			reinterpret_cast<ID2D1Bitmap**>(m_pD2DRenderTarget.GetAddressOf())
		);
	}
}

void D2DApp::CalculateFrameStats()
{
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;
	frameCnt++;

	if ((g_timer.TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = static_cast<float>(frameCnt); // fps = frameCnt / 1
		float mspf = 1000.0f / fps;
		std::ostringstream out;
		out.precision(6);
		out << m_wndCaption
			<< " | " << "FPS: " << fps
			<< " | " << "Frame Time: " << mspf << " ms";
		SetWindowTextA(m_hWnd, out.str().c_str());
		timeElapsed += 1.0f;
		frameCnt = 0;
	}
}

void D2DApp::OnTick(const float& deltaTime)
{
	Tick(deltaTime);
	CalculateFrameStats();
}

HRESULT D2DApp::OnCreate()
{
	// Initialize device-indpendent resources, such as the Direct2D factory.
	HRESULT hr = CreateDeviceIndependentResources();

	// Initialize device-dependent resources.
	if (SUCCEEDED(hr))
	{
		hr = CreateDeviceResources();
	}

	if (SUCCEEDED(hr))
	{
		CreateWindowSizeDependentResources();
		hr = HandleCreateEvent(m_width, m_height) ? S_OK : S_FALSE;
	}

	return hr;
}

LRESULT D2DApp::OnActivate(const WPARAM& nFlags, const HWND& hwnd)
{
	// `nFlags` specifies whether the window is being activated or deactivated. It can be one of the following values.
	//     WA_ACTIVE(1), WA_CLICKACTIVE(2), WA_INACTIVE(0)
	// An application should return zero if it processes this message.
	// @see https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-activate
	if (WA_INACTIVE == nFlags)
	{
		m_bResizing = false;
		m_PressingMouseNum = 0;
		ReleaseCapture();
	}
	return FALSE;
}

LRESULT D2DApp::OnKeyDown(const WPARAM& nKey)
{
	// `nKey` is the virtual-key code of the nonsystem key. @see https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
	// An application should return zero if it processes this message.
	// @see https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-keydown
	HandleKeyDownEvent(nKey);
	return FALSE;
}

LRESULT D2DApp::OnKeyUp(const WPARAM& nKey)
{
	// `nKey`, the same as key down event.
	// An application should return zero if it processes this message.
	// @see https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-keyup
	HandleKeyUpEvent(nKey);
	return FALSE;
}

LRESULT D2DApp::OnLeftMouseDown(const WPARAM& nFlags, const int& x, const int& y)
{
	// `nFlags`, the same as mouse move event.
	// An application should return zero if it processes this message.
	// @see https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-lbuttondown
	HandleLeftMouseDownEvent(nFlags, x, y);
	SetCapture(m_hWnd);
	m_PressingMouseNum++;
	return FALSE;
}

LRESULT D2DApp::OnLeftMouseUp(const WPARAM& nFlags, const int& x, const int& y)
{
	// `nFlags`, the same as mouse move event.
	// An application should return zero if it processes this message.
	// @see https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-lbuttonup
	HandleLeftMouseUpEvent(nFlags, x, y);
	ReleaseCapture();
	m_PressingMouseNum = std::max(0u, m_PressingMouseNum - 1);
	if (0 == m_PressingMouseNum)
	{
		ReleaseCapture();
	}
	return FALSE;
}

LRESULT D2DApp::OnMiddleMouseDown(const WPARAM& nFlags, const int& x, const int& y)
{
	// `nFlags`, the same as mouse move event.
	// An application should return zero if it processes this message.
	// @see https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-mbuttondown
	HandleMiddleMouseDownEvent(nFlags, x, y);
	SetCapture(m_hWnd);
	return FALSE;
}

LRESULT D2DApp::OnMiddleMouseUp(const WPARAM& nFlags, const int& x, const int& y)
{
	// `nFlags`, the same as mouse move event.
	// An application should return zero if it processes this message.
	// @see https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-mbuttonup
	// @note that when a shortcut menu is present (displayed), coordinates are relative to the screen, not the client area.
	//     Because TrackPopupMenu is an asynchronous call and the WM_MBUTTONUP notification does not have a special flag indicating coordinate derivation,
	//     an application cannot tell if the x,y coordinates contained in lParam are relative to the screen or the client area.
	HandleMiddleMouseUpEvent(nFlags, x, y);
	m_PressingMouseNum = std::max(0u, m_PressingMouseNum - 1);
	if (0 == m_PressingMouseNum)
	{
		ReleaseCapture();
	}
	return FALSE;
}

LRESULT D2DApp::OnRightMouseDown(const WPARAM& nFlags, const int& x, const int& y)
{
	// `nFlags`, the same as mouse move event.
	// An application should return zero if it processes this message.
	// @see https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-rbuttondown
	HandleRightMouseDownEvent(nFlags, x, y);
	SetCapture(m_hWnd);
	return FALSE;
}

LRESULT D2DApp::OnRightMouseUp(const WPARAM& nFlags, const int& x, const int& y)
{
	// `nFlags`, the same as mouse move event.
	// An application should return zero if it processes this message.
	// @see https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-rbuttonup
	HandleRightMouseUpEvent(nFlags, x, y);
	m_PressingMouseNum = std::max(0u, m_PressingMouseNum - 1);
	if (0 == m_PressingMouseNum)
	{
		ReleaseCapture();
	}
	return FALSE;
}

LRESULT D2DApp::OnMouseMove(const WPARAM& nFlags, const int& x, const int& y)
{
	// `nFlags` indicates whether various virtual keys are down. This parameter can be one or more of the following values.
	//     MK_CONTROL(0x0008), MK_LBUTTON(0x0001), MK_MBUTTON(0x0010), MK_RBUTTON(0x0002),
	//     MK_SHIFT(0x0004), MK_XBUTTON1(0x0020), MK_XBUTTON2(0x0040)
	// An application should return zero if it processes this message.
	// @see https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-mousemove
	HandleMouseMoveEvent(nFlags, x, y);
	return FALSE;
}

LRESULT D2DApp::OnMouseWheel(const UINT& nFlags, const short& zDelta, const int& x, const int& y)
{
	// `nFlags`, the same as mouse move event.
	// `zDelta` indicates the distance the wheel is rotated, expressed in multiples or divisions of WHEEL_DELTA, which is 120.
	//     A positive value indicates that the wheel was rotated forward, away from the user.
	//     A negative value indicates that the wheel was rotated backward, toward the user.
	// An application should return zero if it processes this message.
	// @see https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-mousewheel
	HandleMouseWheelEvent(nFlags, zDelta, x, y);
	return FALSE;
}

LRESULT D2DApp::OnSizing(const WPARAM& nFlags)
{
	// `nFlags` is the edge of the window that is being sized. It can be one of the following values.
	//     WMSZ_BOTTOM(6), WMSZ_BOTTOMLEFT(7), WMSZ_BOTTOMRIGHT(8), WMSZ_LEFT(1),
	//     WMSZ_RIGHT(2), WMSZ_TOP(3), WMSZ_TOPLEFT(4), WMSZ_TOPRIGHT(5)
	// An application should return TRUE if it processes this message.
	// @see https://docs.microsoft.com/en-us/windows/win32/winmsg/wm-sizing
	m_bResizing = true;
	return TRUE;
}

LRESULT D2DApp::OnSize(const WPARAM& nFlags, const UINT& width, const UINT& height)
{
	// `nFlags` is the type of resizing requested. It can be one of the following values.
	//     SIZE_MAXHIDE(4), SIZE_MAXIMIZED(2), SIZE_MAXSHOW(3), SIZE_MINIMIZED(1), SIZE_RESTORED(0)
	// An application should return zero if it processes this message.
	// @see https://docs.microsoft.com/en-us/windows/win32/winmsg/wm-size
	m_width = width;
	m_height = height;
	m_rect.right = width;
	m_rect.bottom = height;

	if (nFlags == SIZE_MAXIMIZED)
	{
		CreateWindowSizeDependentResources();
		HandleResizeEvent(width, height);
	}
	else if (nFlags == SIZE_RESTORED && m_bWindowMaximum)
	{
		CreateWindowSizeDependentResources();
		HandleResizeEvent(width, height);
	}

	m_bWindowMaximum = (nFlags == SIZE_MAXIMIZED);
	m_bWindowMinimum = (nFlags == SIZE_MINIMIZED);
	return FALSE;
}

LRESULT D2DApp::OnExitSizeMove()
{
	// Sent one time to a window, after it has exited the moving or sizing modal loop.
	// An application should return zero if it processes this message.
	// @see https://docs.microsoft.com/en-us/windows/win32/winmsg/wm-exitsizemove
	if (m_bResizing)
	{
		m_bResizing = false;
		CreateWindowSizeDependentResources();
		HandleResizeEvent(m_width, m_height);
	}
	return FALSE;
}

LRESULT D2DApp::OnDestroy()
{
	// Sent when a window is being destroyed.
	// It is sent to the window procedure of the window being destroyed after the window is removed from the screen.
	// An application should return zero if it processes this message.
	// @see https://docs.microsoft.com/en-us/windows/win32/winmsg/wm-destroy
	HandleDestroyEvent();
	PostQuitMessage(0);
	return FALSE;
}

HRESULT D2DApp::Initialize(HINSTANCE hInstance, int nCmdShow)
{
	HRESULT hr = S_OK;

	// Register window class.
	if (SUCCEEDED(hr))
	{
		WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = D2DApp::WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FREEZERENDER));
		wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW);
		wcex.hbrBackground = nullptr;
		wcex.lpszMenuName = nullptr;
		wcex.lpszClassName = L"D2D1WndClass";
		wcex.hIcon = nullptr;
		RegisterClassExW(&wcex);

		RECT windowRect = { 0, 0, LONG(m_width), LONG(m_height) };
		DWORD windowStyle = WS_OVERLAPPEDWINDOW;
		AdjustWindowRect(&windowRect, windowStyle, FALSE);
		windowRect.right -= windowRect.left;
		windowRect.bottom -= windowRect.top;
		windowRect.left = (::GetSystemMetrics(SM_CXFULLSCREEN) - windowRect.right) / 2;
		windowRect.top = (::GetSystemMetrics(SM_CYFULLSCREEN) - windowRect.bottom) / 2;
		m_hWnd = CreateWindowExA(
			0,
			"D2D1WndClass",
			m_wndCaption,
			windowStyle,
			windowRect.left,
			windowRect.top,
			windowRect.right,
			windowRect.bottom,
			0,
			0,
			hInstance,
			this);

		hr = m_hWnd ? S_OK : E_FAIL;
	}

	if (SUCCEEDED(hr))
	{
		hr = OnCreate();
	}

	if (SUCCEEDED(hr))
	{
		ShowWindow(m_hWnd, nCmdShow);
		UpdateWindow(m_hWnd);
	}
	return hr;
}

void D2DApp::Run()
{
	g_timer.Reset();

	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		g_timer.Tick();
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			OnTick(g_timer.GetDeltaTime());
		}
	}
}

LRESULT D2DApp::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static D2DApp* pThis = nullptr;

	switch (message)
	{
	branch_unlikely case WM_CREATE:
		pThis = static_cast<D2DApp*>(reinterpret_cast<LPCREATESTRUCT>(lParam)->lpCreateParams);
		break;

	branch_unlikely case WM_DESTROY:
		return pThis->OnDestroy();


		case WM_ACTIVATE:       return pThis->OnActivate(wParam, (HWND)lParam);
		case WM_KEYDOWN:        return pThis->OnKeyDown(wParam);
		case WM_KEYUP:          return pThis->OnKeyUp(wParam);

		case WM_LBUTTONDOWN:    return pThis->OnLeftMouseDown(wParam, LOWORD(lParam), HIWORD(lParam));
		case WM_MBUTTONDOWN:    return pThis->OnMiddleMouseDown(wParam, LOWORD(lParam), HIWORD(lParam));
		case WM_RBUTTONDOWN:    return pThis->OnRightMouseDown(wParam, LOWORD(lParam), HIWORD(lParam));

		case WM_LBUTTONUP:      return pThis->OnLeftMouseUp(wParam, LOWORD(lParam), HIWORD(lParam));
		case WM_MBUTTONUP:      return pThis->OnMiddleMouseDown(wParam, LOWORD(lParam), HIWORD(lParam));
		case WM_RBUTTONUP:      return pThis->OnRightMouseUp(wParam, LOWORD(lParam), HIWORD(lParam));

		case WM_MOUSEMOVE:      return pThis->OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		case WM_MOUSEWHEEL:     return pThis->OnMouseWheel(LOWORD(wParam), HIWORD(wParam), LOWORD(lParam), HIWORD(lParam));

		case WM_SIZING:         return pThis->OnSizing(wParam);
		case WM_SIZE:           return pThis->OnSize(wParam, LOWORD(lParam), HIWORD(lParam));
		case WM_EXITSIZEMOVE:   return pThis->OnExitSizeMove();

	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return TRUE;
}