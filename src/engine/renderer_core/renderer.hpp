#pragma once
#include <core/object/object.hpp>
#include <app_framework/base/platform_window.hpp>

namespace ray
{
	
class Vec2d;

enum RenderCommand_t
{
	eNoop,
	eDrawRawVertices
};

template<RenderCommand_t cmd>
class RenderCommand {};

template<RenderCommand_t cmd = eNoop>
class RenderCommand_Noop {};

template<RenderCommand_t cmd = eDrawRawVertices>
class RenderCommand_DrawRawVertices
{
	int Count;
	Vec2d* Vertices;
};

struct IRenderer : public Object
{
	virtual void Initialize(ray::core::IPlatformWindow* window) = 0;
	virtual void Draw() = 0;
	virtual void Shutdown() = 0;
};


}