#include "widget.hpp"
#include "renderer_core/renderer_2d.hpp"

void UiWidget::AddObject(UiObject* object)
{
	_objects.PushBack(object);
	_proxies.PushBack(UiObjectProxy { .Object = object, .Transform = &object->_transform, .RenderData = object->_renderData });
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
	constexpr FVector2 rootObjectPos = {0.f, 0.f};
	
	static CameraActor* camera = new CameraActor;
	
	Renderer2D::SetCamera(*camera);
	
	for (UiObjectProxy& proxy : _proxies)
	{
		UiRenderData& renderData = *proxy.RenderData;

		FVector2 parentTransform = rootObjectPos;
		if (proxy.Object != nullptr)
			parentTransform = proxy.Object->_transform->Position;
		
		FVector2 position2D = parentTransform + renderData.Transform->Position;
		FVector3 position = { position2D.x, position2D.y, 0.1f };
		
		Renderer2D::DrawQuad(position, renderData.Transform->Scale, renderData.TextureIndex, (FVector2*)renderData.TexCoord.GetData(), ctx);
	}
}
