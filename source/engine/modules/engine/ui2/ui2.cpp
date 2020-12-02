#include "ui2.hpp"
#include <renderer_core/command_context.hpp>
#include <renderer_core/renderer_2d.hpp>

using namespace ray::renderer_core_api;

static UiObject gRoot;

UiObject::UiObject()
{
	_parent = &gRoot;
	_render_data = new UiRenderData;
}

void UiRootObject::RenderAll()
{
	GraphicsContext& ctx = GraphicsContext::Begin();
	Renderer2D::Begin(_vp);

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

	Renderer2D::End(ctx);
}

void UiRootObject::AddObject(UiObject* root, UiObject* obj)
{
	obj->_parent = root;
	
	UiObjectsListEntry entry = {
		.Object = obj,
		.Transform = &obj->_render_data->Transform,
		.RenderData = obj->_render_data
	};
	this->_objects.PushBack(entry);
}
