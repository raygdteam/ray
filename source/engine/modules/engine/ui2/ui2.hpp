#pragma once
#include <core/core.hpp>
#include <core/lib/array.hpp>
#include <app_framework/base/platform_window.hpp>
#include <renderer_core/command_context.hpp>

class GraphicsContext;

class UiObject
{
public:
	UiObject();
	virtual void Tick() = 0;
};

class UiWindow
{
	friend class UiRootObject;
protected:
	Array<UiObject*> _objects;
public:
	UiWindow();
};

class UiRootObject
{
	Array<UiWindow*> _windows;
public:
	void Initialize(IPlatformWindow* window);
	void Tick();
	void RenderAll(GraphicsContext& gfxContext);
};
