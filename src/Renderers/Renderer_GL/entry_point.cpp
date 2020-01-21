#include "pch.hpp"
#include "Renderer.hpp"

namespace ray::renderer::detail::opengl
{
	bool is_supported()
	{
		return true;
	}

	IRenderer* create()
	{
		return new Renderer;
	}
}