#include "pch.hpp"
#include "renderer_gl.hpp"

namespace ray::renderer::detail::gl
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
