#include "../renderer_core/renderer.hpp"

//temporary solution
#ifdef RAY_PLATFORM_WIN
#include <Windows.h>

HDC   g_hDC = nullptr;
HGLRC g_hRC = nullptr;

#endif // RAY_PLATFORM_WIN

using namespace ray::core;

class RendererGL : public ray::IRenderer
{
public:
	void Initialize(IPlatformWindow* window) override;
	void Draw() override;
	void Shutdown() override;

private:
	IPlatformWindow* _window;
	
};

void RendererGL::Initialize(IPlatformWindow* window)
{
	_window = window;
	
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 16;
	pfd.cDepthBits = 16;

	g_hDC = GetDC(static_cast<HWND>(window->GetWindowHandleRaw()));
	
	//you need to include necessary libraries
	GLuint iPixelFormat = ChoosePixelFormat(g_hDC, &pfd);

	if (iPixelFormat != 0)
	{
		PIXELFORMATDESCRIPTOR bestMatch_pfd;
		DescribePixelFormat(g_hDC, iPixelFormat, sizeof(pfd), &bestMatch_pfd);

		if (bestMatch_pfd.cDepthBits < pfd.cDepthBits)
		{
			return;
		}

		if (SetPixelFormat(g_hDC, iPixelFormat, &pfd) == FALSE)
		{
			DWORD dwErrorCode = GetLastError();
			return;
		}
	}
	else
	{
		DWORD dwErrorCode = GetLastError();
		return;
	}

	g_hRC = wglCreateContext(g_hDC);
	wglMakeCurrent(g_hDC, g_hRC);

	//you need to include necessary libraries
	glClearColor(0.0f, 1.0f, 0.0f, 1.0f); // ÷вет вьюпорта OpenGL
}

void RendererGL::Draw()
{}

void RendererGL::Shutdown()
{
	if (g_hRC != nullptr)
	{
		wglMakeCurrent(nullptr, nullptr);
		wglDeleteContext(g_hRC);
		g_hRC = nullptr;
	}

	if (g_hDC != nullptr)
	{
		ReleaseDC(static_cast<HWND>(_window->GetWindowHandleRaw()), g_hDC);
		g_hDC = nullptr;
	}
}

extern "C" __declspec(dllexport) ray::IRenderer*  GetRendererApi()
{
	return new RendererGL();
}