#pragma once
#include "renderer.hpp"
#include <core/core.hpp>

namespace ray::renderer_core_api
{
	struct Vertex
	{
		float position[3];
	};

	enum class PrimitiveTopology
	{
		point, 
		line, 
		triangle
	};

	class RendererCommands
	{
	public:
		static bool Initialize(IRenderer* renderer);

		static void Draw(Vertex* vertices, PrimitiveTopology topology);
		static void DrawIndexed(Vertex* vertices, u32* indices);

	};
}

