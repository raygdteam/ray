#pragma once
#include <core/core.hpp>

#include <engine/ui/ui_render_data.hpp>
#include <engine/world/components/transform.hpp>

class UiObject
{
	friend class UiWidget;
protected:
	Transform* _transform;
	UiRenderData* _renderData;
	
public:
	UiObject() {};
	virtual ~UiObject() {};

	virtual void Tick() = 0;
	virtual void Render() = 0;
};
