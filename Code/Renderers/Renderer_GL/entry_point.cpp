#include "pch.hpp"
#include "renderer_gl.hpp"

namespace ray::renderer::detail::opengl
{
	bool is_supported()
	{
		return true;
	}

	IRenderer* create()
	{
		return new RendererGL;
	}
}