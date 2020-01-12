#include "pch.hpp"
#include "RendererDX.hpp"

namespace ray::renderer::detail::dx
{

	bool is_supported()
	{
		return true;
	}

	IRenderer* create()
	{
		return new RendererDX;
	}

}
