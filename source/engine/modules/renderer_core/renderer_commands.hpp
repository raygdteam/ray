#pragma once
#include "math/vector3.hpp"
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

		void Draw(Vertex* vertices, size_t vertexCount);
		void DrawIndexed(Vertex* vertices, size_t vertexCount, u32* indices, size_t indexCount);

	private:
		IDevice* _device;
		IRRCClassHelper* _classHelper;
		IRootSignature* _rootSignature;
		IPipelineState* _pipelineState;
		IVertexShader* _vertexShader;
		IPixelShader* _pixelShader;
		resources::IVertexBuffer* _vertexBuffer;
		resources::IIndexBuffer* _indexBuffer;
		resources::IResourceBarrier* _resourceBarrier;
		ICommandList* _commandList;
		ICommandAllocator* _3dCommandAllocator;
		ICommandAllocator* _copyCommandAllocator; // is not used yet
		ICommandAllocator* _computeCommandAllocator; // is not used yet
	};
}

