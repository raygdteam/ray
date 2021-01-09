#pragma once
#include <core/core.hpp>
#include <core/lib/array.hpp>
#include <core/math/vector.hpp>
#include <core/math/matrix.hpp>
#include <app_framework/base/platform_window.hpp>
#include <engine/world/components/transform.hpp>

class UiObject
{
public:
	UiObject();
	virtual void Tick() = 0;
};

class UiButton : public UiObject
{
public:
	void Tick() override;
};

class UiRootObject
{
public:
	void Initialize(IPlatformWindow* window);
	void Tick();
	void RenderAll();
};
