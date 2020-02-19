#include "base/platform_window.hpp"
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <ray/os/include.hpp>
#include <functional>
#undef CreateWindow

class PlatformWindow : public ray::core::IPlatformWindow
{
	HWND _windowHandle = nullptr;
public:
	void Initialize() override;
	bool CreateWindow(const char* name) override;
	void SetWindowVisibility(bool visible) override;
	void Update() override;
	void* GetWindowHandleRaw() override;
	void Destroy() override;
	void Shutdown() override;
};

void PlatformWindow::Initialize()
{
	WNDCLASS wc = {};

	wc.hInstance = GetModuleHandleA(nullptr);
	wc.lpszClassName = "RAY_ENGINE";
	wc.lpfnWndProc = DefWindowProc;

	RegisterClass(&wc);
}

bool PlatformWindow::CreateWindow(const char* name)
{
	_windowHandle = CreateWindowA("RAY_ENGINE", name, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
								CW_USEDEFAULT, nullptr, nullptr, GetModuleHandleA(0), 0);

	UpdateWindow(_windowHandle);

	return _windowHandle != nullptr;
}

void PlatformWindow::SetWindowVisibility(bool visible)
{
	ShowWindow(_windowHandle, visible ? SW_SHOW : SW_HIDE);
}

void PlatformWindow::Update()
{
	static MSG msg;
	if (PeekMessage(&msg, _windowHandle, 0,0,PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void* PlatformWindow::GetWindowHandleRaw()
{
	return _windowHandle;
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

