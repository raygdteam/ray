#include <core/core.hpp>
// TODO: fixit
#include "../base/platform_window.hpp"
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <windows.h>
#include <functional>

#undef CreateWindow

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
bool gIsClosed = false;

class PlatformWindow : public ray::core::IPlatformWindow
{
	HWND _windowHandle = nullptr;
	MSG _lastMsg;
	u16 _width = 1280;
	u16 _height = 720;
	bool _osRequestedClose = false;
	MulticastDelegate<void(void*, u32, u64, s64)> _cb;
public:
	void Initialize() override;
	bool CreateWindow(const char* name) override;
	void SetWindowVisibility(bool visible) override;
	void Update() override;
	bool ShouldClose() override;
	void* GetWindowHandleRaw() override;
	u16 GetWidth() override;
	u16 GetHeight() override;
	void Destroy() override;
	void Shutdown() override;
	void RegisterEventCallback(Function<void(void*, u32, u64, s64)> callback) override;
	void ProcessCallback(void* wnd, u32 msg, u64  param1, s64 param2);
};

void PlatformWindow::Initialize()
{	
	WNDCLASSEXA wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(nullptr);
	wc.hIcon = LoadIcon(GetModuleHandle(nullptr), IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = "RAY_ENGINE";
	wc.hIconSm = LoadIcon(GetModuleHandle(nullptr), IDI_APPLICATION);

	RegisterClassExA(&wc);
}

bool PlatformWindow::CreateWindow(const char* name)
{
	_windowHandle = CreateWindowA("RAY_ENGINE", name, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, _width,
								_height, nullptr, nullptr, GetModuleHandleA(0), 0);

	UpdateWindow(_windowHandle);

	return _windowHandle != nullptr;
}

void PlatformWindow::SetWindowVisibility(bool visible)
{
	ShowWindow(_windowHandle, visible ? SW_SHOW : SW_HIDE);
	SetWindowLongPtrA(_windowHandle, GWLP_USERDATA, (LONG_PTR)this);
}

void PlatformWindow::Update()
{
	MSG msg = {};
	if (PeekMessage(&msg, this->_windowHandle, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (gIsClosed)
			msg.message = WM_CLOSE;
		this->_lastMsg = msg;

		if (msg.message == WM_CLOSE || msg.message == WM_DESTROY)
			_osRequestedClose = true;
	}
}

bool PlatformWindow::ShouldClose()
{
	return _osRequestedClose;
}

void* PlatformWindow::GetWindowHandleRaw()
{
	return _windowHandle;
}

u16 PlatformWindow::GetWidth()
{
	return _width;
}

u16 PlatformWindow::GetHeight()
{
	return _height;
}

void PlatformWindow::Destroy()
{
	DestroyWindow(_windowHandle);
}

void PlatformWindow::Shutdown()
{ }

ray::core::IPlatformWindow* ray::core::IPlatformWindow::CreateInstance()
{
	return new PlatformWindow();
}

void PlatformWindow::RegisterEventCallback(Function<void(void*, u32, u64, s64)> callback)
{
	_cb.Register(callback);
}

void PlatformWindow::ProcessCallback(void* wnd, u32 msg, u64 param1, s64 param2)
{
	_cb.Invoke(_windowHandle, msg, param1, param2);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PlatformWindow* window = reinterpret_cast<PlatformWindow*>(GetWindowLongPtrA(hwnd, GWLP_USERDATA));
	if (window)
	{
		window->ProcessCallback(static_cast<void*>(hwnd), msg, wParam, lParam);
	}

	switch (msg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		gIsClosed = true;
		break;
	/*case WM_DESTROY:
		PostQuitMessage(0);
		break;*/
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
