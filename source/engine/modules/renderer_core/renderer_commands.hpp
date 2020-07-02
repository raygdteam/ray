#pragma once
#include "renderer.hpp"
#include "math/vectors.hpp"
#include <core/core.hpp>

namespace ray::renderer_core_api
{
	struct Vertex
	{
		Vertex() {}
		Vertex(const math::Vector3& pos) : _position(pos) {}
		Vertex(const Vertex& other) {}
		Vertex(Vertex&& other) {}

		math::Vector3 _position;

		static size_t GetSize() { return sizeof(Vertex); }
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
		bool Initialize(IRenderer* renderer, PrimitiveTopology topology = PrimitiveTopology::undefined);

		void Draw(Vertex* vertices);
		void DrawIndexed(Vertex* vertices, u32* indices);

	private:
		IDevice* _device;

	};
}

