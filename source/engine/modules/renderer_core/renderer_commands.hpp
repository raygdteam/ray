#pragma once
#include "math/vectors.hpp"
#include "renderer.hpp"
#include <vector>

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

	class RendererCommands
	{
	public:
		bool Initialize(IRenderer* renderer, PrimitiveTopology topology = PrimitiveTopology::eUndefined);

		static void BeginScene();
		static void EndScene();

		static void Execute();

		void Draw(Vertex* vertices, size_t vertexCount);
		void DrawIndexed(Vertex* vertices, size_t vertexCount, u32* indices, size_t indexCount);

	private:
		IDevice* _device;
		IRRCClassHelper* _class_helper;
		IRootSignature* _root_signature;
		IPipelineState* _pipeline_state;
		IVertexShader* _vertex_shader;
		IPixelShader* _pixel_shader;
		resources::IVertexBuffer* _vertex_buffer;
		resources::IIndexBuffer* _index_buffer;
		resources::IResourceBarrier* _resource_barrier;
		ICommandList* _command_list;
		static std::vector<ICommandList*> _lists;
		static ICommandQueue* _command_queue;
	};
}

