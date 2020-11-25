#include "ui2.hpp"
#include <renderer_core/command_context.hpp>
#include <renderer_core/renderer_2d.hpp>

using namespace ray::renderer_core_api;

void UiRootObject::RenderAll()
{
	GraphicsContext& ctx = /* представьте, что тут самый лучший способ получения ctx */;

	for (UiObjectsListEntry& object : _objects)
	{
		/*
		 * Видит [object] в реке [object]
		 * Сунул [object] [object] в [object]
		 * [object] [object] [object] [object] 
		 */
		FVector2 pos = object.Object->GetParent()._render_data->Transform->Position + object.Object->_render_data->Transform->Position;
		UiRenderData* data = object.RenderData;

		Renderer2D::DrawQuad(FVector3::FromVector2(pos), data->Transform->Scale, data->TextureIndex, ctx);
	}
}
