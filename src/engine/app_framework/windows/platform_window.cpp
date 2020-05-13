#include "base/platform_window.hpp"
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <ray/os/include.hpp>
#include <functional>

#undef CreateWindow

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
bool gIsClosed = false;

class PlatformWindow : public ray::core::IPlatformWindow
{
	HWND _windowHandle = nullptr;
	MSG _lastMsg;
public:
	void Initialize() override;
	bool CreateWindow(const char* name) override;
	void SetWindowVisibility(bool visible) override;
	void Update() override;
	bool ShouldClose() override;
	void* GetWindowHandleRaw() override;
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
	_windowHandle = CreateWindowA("RAY_ENGINE", name, WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
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
		if (gIsClosed)
			msg.message = WM_CLOSE;
		_lastMsg = msg;
	}
}

bool PlatformWindow::ShouldClose()
{
	return _lastMsg.message == WM_CLOSE || _lastMsg.message == WM_DESTROY;
}

void* PlatformWindow::GetWindowHandleRaw()
{
	return _windowHandle;
}

void PlatformWindow::Destroy()
{
	//DestroyWindow(_windowHandle);
	FreeConsole();
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
