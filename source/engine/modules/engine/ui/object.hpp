#pragma once
#include <core/core.hpp>

#include <engine/ui/ui_render_data.hpp>
#include <engine/world/components/transform.hpp>

class UiObject
{
	friend class UiWidget;
	friend class UiButton; // TODO: notlikethis
protected:
	UiObject* _parent;
	UiWidget* _parentWidget;
	
	Transform* _transform;
	UiRenderData* _renderData;

	void SpawnSubobject(UiObject* object)
	{
		// TODO:
	}
	
public:
	UiObject() {};
	virtual ~UiObject() {};

	const Array<UiObject&>& GetChildren() const noexcept;
	const void AddChildren(UiObject* object);

	virtual void Tick() = 0;
	virtual void Render() = 0;
};
