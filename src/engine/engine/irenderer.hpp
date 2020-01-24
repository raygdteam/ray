#pragma once

namespace ray::renderer 
{
	enum eRendererType
	{
		Null,
		Dx11,
		OpenGL,
		Vk
	};

	class IRenderer
	{
	public:
		virtual ~IRenderer() = 0;

		virtual bool Init() = 0;
		virtual void Destroy() = 0;
		virtual void Draw() = 0;
		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;

		virtual void WindowSizeChanged(u32 width, u32 height) = 0;

		static IRenderer* create_renderer(eRendererType type);
	};

	IRenderer* CreateRenderer();

}