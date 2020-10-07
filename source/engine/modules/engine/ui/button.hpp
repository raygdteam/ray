#pragma once

#include <renderer_core/renderer_2d.hpp>

bool button(const FVector<3>& pos, const FVector<2>& size, FVector<2>* textureCoords, ray::renderer_core_api::GraphicsContext& gfxContext)
{
	ray::renderer_core_api::Renderer2D::DrawQuad(pos, size, textureCoords, gfxContext);
}
