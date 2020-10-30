#include "widget.hpp"
#include "renderer_core/renderer_2d.hpp"

void UiWidget::AddObject(UiObject* object)
{
	_objects.PushBack(object);
	_proxies.PushBack(UiObjectProxy { .Transform = &object->_transform, .RenderData = object->_renderData });
}

void UiWidget::RemoveObject(UiObject* object)
{
	_objects.erase(&object);
}

void UiWidget::Update()
{
	// TODO: ?
	// #pragma omp parallel for
	for (UiObject* object : _objects)
	{
		object->Tick();
	}
}

void UiWidget::RenderAll(GraphicsContext& ctx)
{
	for (UiObjectProxy& proxy : _proxies)
	{
		UiRenderData& renderData = *proxy.RenderData;

		FVector3 position = FVector3 { proxy.Object->_transform->Position.x, proxy.Object->_transform->Position.y } + renderData.Position;
		
		Renderer2D::DrawQuad(position, renderData.Scale, renderData.TextureIndex, (FVector2*)renderData.TexCoord.GetData(), ctx);
	}
}
