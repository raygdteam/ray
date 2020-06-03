#pragma once
#include <core/object/object.hpp>
#include <app_framework/base/platform_window.hpp>

namespace ray
{
	
struct IRenderer : public Object
{
	virtual void Initialize(ray::core::IPlatformWindow* window) = 0;
	virtual void Draw() = 0;
	virtual void Shutdown() = 0;
};

}