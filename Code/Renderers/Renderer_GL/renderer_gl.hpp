#pragma once
#ifdef RAY_PLATFORM_WIN
#include <Windows.h>
#endif

class RendererGL :
	public ray::renderer::IRenderer
{
public:
	RendererGL();
	~RendererGL() override;

	void Draw() override;
	bool Init() override;
	void Destroy() override;
	void BeginFrame() override;
	void EndFrame() override;

private:
	HDC m_hDC;
	HGLRC m_hRC;

};

