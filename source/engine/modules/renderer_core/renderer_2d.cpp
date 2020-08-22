#include "renderer_2d.hpp"
#include "ray_renderer_core_class_helper.hpp"
#include <engine/state/state.hpp>
#include "resources/gpu_buffer.hpp"
#include "pipeline_state.hpp"
#include "root_signature.hpp"

namespace ray::renderer_core_api
{
	struct QuadVertex
	{
		FVector<3> Position;
		FVector<4> Color;
	};

	struct Renderer2DData
	{
		Renderer2DData() {}

		static const uint32_t MAX_QUADS = 20000;
		static const uint32_t MAX_VERTICES = MAX_QUADS * 4;
		static const uint32_t MAX_INDICES = MAX_QUADS * 6;

		resources::GpuBuffer VertexAndIndexBuffer;
		GraphicsPipeline _2DPipeline;
		RootSignature _2DSignature;

		u32 QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		FVector<4> QuadVertexPositions[4];
	};

	static Renderer2DData sData;

	void Renderer2D::Initialize()
	{
	}

	void Renderer2D::Begin()
	{
		
	}

	void Renderer2D::End()
	{
		Flush();
	}

	void Renderer2D::DrawQuad(FVector<3> pos, FVector<4> color)
	{
		

	}

	void Renderer2D::Flush()
	{

	}

	void Renderer2D::FlushAndReset()
	{
		Flush();
		Begin();
	}

	Renderer2D::~Renderer2D()
	{

	}
}