#include "pch.hpp"
#include "Renderer.hpp"

#include "Classes/Shader/Shader.hpp"
#include "Classes/Shader/Program.hpp"

#include "Classes/Model/Model.hpp"

#include "Classes/Texture/texture.hpp"

Program* program;
Texture* texture;
GLuint vbo, vao, ebo;

bool Renderer::Init()
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
			return false;

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

	program = new Program();
	auto vertexShader = new Shader("/resources/shaders/opengl/vs.glsl", GL_VERTEX_SHADER);
	auto fragmentShader = new Shader("/resources/shaders/opengl/fs.glsl", GL_FRAGMENT_SHADER);

	vertexShader->Compile();
	fragmentShader->Compile();

	program->Attach(vertexShader);
	program->Attach(fragmentShader);

	program->Link();

	texture = new Texture("/resources/textures/wall.jpg");

	float vertices[] = {
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f
	};
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	return true;
}

void Renderer::Destroy()
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

void Renderer::BeginFrame()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5f, 0.0f, 0.5f, 1.f);
}

void Renderer::Draw()
{
	texture->Bind();
	program->Use();
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Renderer::EndFrame()
{
	SwapBuffers(m_hDC);
}

void Renderer::WindowSizeChanged(u32 width, u32 height)
{
	glViewport(0, 0, width, height);
}