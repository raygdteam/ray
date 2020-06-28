#pragma once
#include "renderer.hpp"
#include "math/vectors.hpp"
#include <core/core.hpp>

namespace ray::renderer_core_api
{
	struct Vertex
	{
		math::Vector3 _position;
		math::Vector4 _color;
	};

	enum class PrimitiveTopology
	{
		undefined, //can't draw anything
		point, 
		line, 
		triangle,
		patch //for tesselation
	};

	class RendererCommands
	{
	public:
		static bool Initialize(IRenderer* renderer);

		static void Draw(Vertex* vertices, PrimitiveTopology topology = PrimitiveTopology::undefined);
		static void DrawIndexed(Vertex* vertices, u32* indices);

	};
}

