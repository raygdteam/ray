#pragma once
#include <core/core.hpp>
#include <core/lib/array.hpp>

#include <engine\ui\object.hpp>
#include <engine/ui/ui_render_data.hpp>
#include <engine/world/components/transform.hpp>

#include <renderer_core/command_context.hpp>

using namespace ray::renderer_core_api;

struct UiObjectProxy
{
	Transform** Transform;
	UiRenderData* RenderData;
};

class UiWidget
{
	Array<UiObject*> _objects;
	Array<UiObjectProxy> _proxies;
	
public:
	void AddObject(UiObject*);
	void RemoveObject(UiObject*); // ??

	void Update();
	void RenderAll(GraphicsContext&);
};
