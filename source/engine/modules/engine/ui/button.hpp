#pragma once

#include <renderer_core/renderer_2d.hpp>

#include <input/input.hpp>

namespace ui
{
	/*bool button(const FVector<3>& pos, const FVector<2>& size, FVector<2>* textureCoords, ray::renderer_core_api::GraphicsContext& gfxContext)
	{
		ray::renderer_core_api::Renderer2D::DrawQuad(pos, size, textureCoords, gfxContext);
	}*/

	bool button(const FVector<2>& position, const FVector<2>& size)
	{
		return input::mouse::x() >= position.x && input::mouse::x() <= size.x && input::mouse::y() >= position.y && input::mouse::y() <= size.y;
	}
}
