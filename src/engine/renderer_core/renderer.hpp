#pragma once
#include "core/object/object.hpp"

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

struct IRenderer : public core::Object
{
	virtual void Initialize(void* window) = 0;
	virtual void Draw() = 0;
};


}