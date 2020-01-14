#include "pch.hpp"
#include "renderer_gl.hpp"

#include "classes/vertex/vertex.hpp"

bool RendererGL::Init()
{
	glEnable(GL_DEPTH_TEST);

#ifdef RAY_PLATFORM_WIN
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 16;
	pfd.cDepthBits = 16;

	m_hDC = GetDC(Platform::GetHWND());
	GLuint iPixelFormat = ChoosePixelFormat(m_hDC, &pfd);

	if (iPixelFormat != 0)
	{
		PIXELFORMATDESCRIPTOR bestMatch_pfd;
		DescribePixelFormat(m_hDC, iPixelFormat, sizeof(pfd), &bestMatch_pfd);

		if (bestMatch_pfd.cDepthBits < pfd.cDepthBits)
		{
			return false;
		}

		if (SetPixelFormat(m_hDC, iPixelFormat, &pfd) == FALSE)
		{
			DWORD dwErrorCode = GetLastError();
			return false;
		}
	}
	else
	{
		DWORD dwErrorCode = GetLastError();
		return false;
	}

	m_hRC = wglCreateContext(m_hDC);
	wglMakeCurrent(m_hDC, m_hRC);

#endif

	glewInit();

	float points[] = {
0.0f,  0.5f,  0.0f,
0.5f, -0.5f,  0.0f,
-0.5f, -0.5f,  0.0f
	};

	return true;
}

void RendererGL::Destroy()
{
	if (m_hRC != NULL)
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(m_hRC);
		m_hRC = NULL;
	}

	if (m_hDC != NULL)
	{
		ReleaseDC(Platform::GetHWND(), m_hDC);
		m_hDC = NULL;
	}
}

void RendererGL::BeginFrame()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RendererGL::Draw()
{
}

void RendererGL::EndFrame()
{
	SwapBuffers(m_hDC);
}
