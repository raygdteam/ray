#pragma once
#include <core/core.hpp>
#include <engine/engine/engine_def.hpp>
#include <core/lib/array.hpp>
#include <app_framework/base/platform_window.hpp>
#include <renderer_core/command_context.hpp>

class GraphicsContext;

class RAY_ENGINE_API UiObject
{
public:
	virtual ~UiObject() = default;
	UiObject();
	virtual void Tick() = 0;
};

class RAY_ENGINE_API UiWindow
{
	friend class UiRootObject;
protected:
	Array<UiObject*> _objects;
	String Title;
public:
	UiWindow() {}
};

class RAY_ENGINE_API UiRootObject
{
	Array<UiWindow*> _windows;
public:
	void Initialize(IPlatformWindow* window);
	void Tick();
	void RenderAll(GraphicsContext& gfxContext);

	void AddWindow(UiWindow* window);
};
