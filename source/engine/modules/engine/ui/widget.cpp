#include "widget.hpp"

void UiWidget::AddObject(UiObject* object)
{
	_objects.PushBack(object);
	_proxies.PushBack(UiObjectProxy { .Transform = &object->_transform, .RenderData = object->_renderData });
}

void UiWidget::RemoveObject(UiObject*)
{
}

void UiWidget::Update()
{
	for (UiObject* object : _objects)
	{
		object->Tick();
	}
}

void UiWidget::RenderAll(GraphicsContext& ctx)
{
	Array<QuadVertex> vertices;

	for (UiObjectProxy& proxy : _proxies)
	{
		UiRenderData& renderData = *proxy.RenderData;
		
		for (u64 i = 0; i < renderData.StateDescriptions; ++i)
		{
			UiRenderDataState& state = renderData.StateDescriptions[i];

			for (u64 ii = 0; ii < state.NumVertices; ++ii)
			{
				vertices.PushBack(state.Vertices[ii]);
			}
		}
	}

	ctx.SetDynamicVB(0, vertices.Size(), 12, vertices.GetData());
}
