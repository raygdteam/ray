#pragma once
#include <core/core.hpp>
#include <engine/engine/engine_def.hpp>
#include <core/lib/array.hpp>
#include <core/math/vector.hpp>
#include <app_framework/base/platform_window.hpp>
#include <renderer_core/command_context.hpp>


class GraphicsContext;
class UiWindow;
class UiObject;

class RAY_ENGINE_API UiObject
{
protected:
	UiWindow* _parent = nullptr;
public:
	virtual ~UiObject() = default;
	UiObject(UiWindow* parent);
	virtual void Tick() = 0;
};

class RAY_ENGINE_API UiWindow
{
	friend class UiRootObject;
	friend class UiObject;
	void TickBegin();
protected:
	String Title;
	Array<UiObject*> _objects;
	FVector2 Size;
	FVector2 Padding = {-1.f, -1.f};
	bool _dockingEnabled = false;
	
	virtual void DockingOneTimeSetup() {}
public:
	UiWindow() {}

	virtual void Tick() {}
	virtual void LateTick() {}
	
	FVector2 GetSize() const noexcept
	{
		return Size;
	}

	bool IsDockingEnabled()
	{
		return _dockingEnabled;
	}
};

class RAY_ENGINE_API UiRootObject
{
	Array<UiWindow*> _windows;
	bool _dockspaceEnabled = true;
public:
	void Initialize(IPlatformWindow* window);
	void Tick();
	void RunDockspace();
	void RenderAll(GraphicsContext& gfxContext);

	void AddWindow(UiWindow* window);

	void SetDockspaceEnabled(bool state);
};
