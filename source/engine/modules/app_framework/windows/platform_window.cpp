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
};

void PlatformWindow::Initialize()
{
	WNDCLASS wc = {};
	
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.hInstance = GetModuleHandleA(nullptr);
	wc.lpszClassName = "RAY_ENGINE";
	wc.lpfnWndProc = WndProc;

	RegisterClass(&wc);
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
}

void PlatformWindow::Update()
{
	MSG msg;
	if (PeekMessage(&msg, _windowHandle, 0,0,PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (gIsClosed)
			msg.message = WM_CLOSE;
		_lastMsg = msg;

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

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
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
	return 0;
}
