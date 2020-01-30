#include "base/platform_window.hpp"
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <ray/os/include.hpp>
#undef CreateWindow

/*
 * Workaround for this linker error:
 *   error LNK2001: unresolved external symbol _purecall
 */
int __cdecl _purecall() { return 0; }

class PlatformWindow : public ray::core::IPlatformWindow
{
	HWND _windowHandle = nullptr;
public:
	void Initialize() override;
	bool CreateWindow(char name[64]) override;
	void SetWindowVisibility(bool visible) override;
	void* GetWindowHandleRaw() override;
	void Destroy() override;
	void Shutdown() override;
};

void PlatformWindow::Initialize()
{
	WNDCLASS wc = {};

	wc.hInstance = GetModuleHandleA(nullptr);
	wc.lpszClassName = L"RAY_ENGINE";
	wc.lpfnWndProc = DefWindowProc;

	RegisterClass(&wc);
}

bool PlatformWindow::CreateWindow(char name[64])
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
