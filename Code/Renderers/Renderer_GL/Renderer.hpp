#pragma once

class Renderer : public ray::renderer::IRenderer
{
public:
	void WindowSizeChanged(u32 width, u32 height) override {}

	void Draw() override;
	bool Init() override;
	void Destroy() override;
	void BeginFrame() override;
	void EndFrame() override;

private:
	HDC m_hDC;
	HGLRC m_hRC;
};