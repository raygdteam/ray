#include "pch.h"
#include "irenderer.hpp"

#ifdef RAY_PLATFORM_WIN
//#include "../Renderers/Renderer_DX11/RendererDX.hpp"
//#include "../Renderers/Renderer_Vk/VulkanRenderer.hpp"

#include <map>

//using DefaultRenderer = RendererDX;
//using DefaultRenderer = ray::renderers::detail::Renderer_Vk;

#else
#include "../Renderers/Renderer_GL/renderer_gl.hpp"

using DefaultRenderer = RendererGL;

#endif // RAY_PLATFORM_WIN

namespace ray::renderer::detail
{
	namespace vk
	{
		bool is_supported();
		IRenderer* create();
	}

	namespace dx
	{
		bool is_supported();
		IRenderer* create();
	}

	namespace opengl
	{
		bool is_supported();
		IRenderer* create();
	}
}

class NullRenderer : public ray::renderer::IRenderer
{
public:
	void WindowSizeChanged(u32 width, u32 height) override {}
	bool Init() override { return true; }
	void Destroy() override {}
	void Draw() override {}
	void BeginFrame() override {}
	void EndFrame() override {}
};

namespace ray::renderer
{

	IRenderer::~IRenderer() {}
	
	IRenderer* IRenderer::create_renderer(eRendererType type)
	{
		IRenderer* renderer = nullptr;

		switch (type)
		{
		case Vk:
			renderer = detail::vk::create();
			break;
		case Dx11:
			renderer = detail::dx::create();
			break;
		case OpenGL:
			renderer = detail::opengl::create();
			break;
		case Null: 
			renderer = new NullRenderer;
			break;
		default: 
			// ignore
			;
		}

		spdlog::debug("renderer: created renderer at {}", (void*)renderer);
		return renderer;
	}
	

	IRenderer* CreateRenderer()
	{
		return nullptr;
		//return new DefaultRenderer;
	}
}
