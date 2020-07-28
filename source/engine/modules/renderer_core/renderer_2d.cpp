#include "renderer_2d.hpp"
#include "ray_renderer_core_class_helper.hpp"
#include <engine/state/state.hpp>

namespace ray::renderer_core_api
{
	IRRCClassHelper* gClassHelper = nullptr;

	struct QuadVertex
	{
		math::Vector3 Position;
		math::Vector4 Color;
	};

	struct Renderer2DData
	{
		const size_t MAX_QUAD_COUNT = 100;
		const size_t MAX_VERTEX_COUNT = MAX_QUAD_COUNT * 4;
		const size_t MAX_INDEX_COUNT = MAX_QUAD_COUNT * 6;

		resources::IVertexBuffer* VertexBuffer;
		resources::IIndexBuffer* IndexBuffer;
		IPipelineState* PipelineState;

		QuadVertex* VertexData;
		u32* IndexData;
		size_t VertexBufferSize;
		size_t IndexCount;
		math::Vector4 DefaultVertexPosition[4];
	};

	Renderer2DData gRenderData;

	void Renderer2D::Initialize(IDevice* device, IModule* rendererModule)
	{
		_device = device;

		gClassHelper = static_cast<IRRCClassHelper*>(rendererModule->QueryModuleInterface());

		_commandList = gClassHelper->CreateCommandList();
		_commandAllocator = gClassHelper->CreateCommandAllocator();

		gRenderData.VertexBufferSize = 0;
		gRenderData.IndexCount = 0;
		gRenderData.VertexData = new QuadVertex[gRenderData.MAX_VERTEX_COUNT];
		gRenderData.IndexData = new u32[gRenderData.MAX_INDEX_COUNT];

		u32 offset = 0;
		for (u32 i = 0; i < gRenderData.MAX_INDEX_COUNT; i += 6)
		{
			gRenderData.IndexData[i + 0] = offset + 0;
			gRenderData.IndexData[i + 1] = offset + 1;
			gRenderData.IndexData[i + 2] = offset + 2;

			gRenderData.IndexData[i + 3] = offset + 2;
			gRenderData.IndexData[i + 4] = offset + 3;
			gRenderData.IndexData[i + 5] = offset + 0;

			offset += 4;
		}

		gRenderData.DefaultVertexPosition[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		gRenderData.DefaultVertexPosition[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		gRenderData.DefaultVertexPosition[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		gRenderData.DefaultVertexPosition[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::Begin()
	{
		gRenderData.VertexBufferSize = 0;
		gRenderData.IndexCount = 0;
	}

	void Renderer2D::End()
	{
		Flush();
	}

	void Renderer2D::DrawQuad(math::Vector3 pos, math::Vector4 color)
	{
		const size_t quadVertexCount = 4;

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			QuadVertex* ptr = gRenderData.VertexData + gRenderData.VertexBufferSize;
			ptr->Position = pos * gRenderData.DefaultVertexPosition[i];
			ptr->Color = color;

			gRenderData.VertexBufferSize++;
		}

		gRenderData.IndexCount += 6;

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