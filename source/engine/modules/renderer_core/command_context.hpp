#pragma once
#include "command_queue.hpp"
#include "resources/linear_allocator.hpp"
#include "pipeline_state.hpp"
#include "resources/color_buffer.hpp"
#include <core/threading/critical_section.hpp>
#include <d3d12.h>
#include <core/core.hpp>
#include "resources/depth_buffer.hpp"

#ifdef RAY_BUILD_RENDERER_CORE
#define RAY_RENDERERCORE_API __declspec(dllexport)
#else
#define RAY_RENDERERCORE_API RAY_DLLIMPORT
#endif

namespace ray::renderer_core_api
{

	struct NonCopyable
	{
		NonCopyable() = default;
		NonCopyable(const NonCopyable&) = delete;
		NonCopyable& operator = (const NonCopyable&) = delete;
	};

	class CommandContext;
	class ComputeContext;
	class GraphicsContext;

	class RAY_RENDERERCORE_API ContextManager
	{
	public:
		ContextManager() {}

		CommandContext* AllocateContext(D3D12_COMMAND_LIST_TYPE type) noexcept;
		void FreeContext(CommandContext* context) noexcept;
		static void DestroyAllContexts() noexcept;

	private:
		static std::vector<CommandContext*> _sContextPool[4];
		static std::queue<CommandContext*> _sAvailableContexts[4];
		static ray::CriticalSection _sContextManagerMutex;
	};

	class CommandContext : public NonCopyable
	{
		friend class ContextManager;

	private:
		CommandContext(D3D12_COMMAND_LIST_TYPE type);
		
	public:
		~CommandContext();

		static CommandContext& Begin();
		static void DestroyAllContexts();

		// 
		u64 Flush(bool bWaitForComplition = false);

		u64 Finish(bool bWaitForComplition = false);

		void Initialize();
		void Reset();

		GraphicsContext& GetGraphicsContext() noexcept
		{
			assert(_type != D3D12_COMMAND_LIST_TYPE_COMPUTE);
			return reinterpret_cast<GraphicsContext&>(*this);
		}

		ComputeContext& GetComputeContext() noexcept
		{
			return reinterpret_cast<ComputeContext&>(*this);
		}

		ID3D12CommandList* GetCommandList() const noexcept
		{
			return _commandList;
		}

		void CopyBuffer(resources::GpuResource& dest, resources::GpuResource& src);
		void CopyBufferRegion(resources::GpuResource& dest, size_t destOffset, resources::GpuResource& src, size_t srcOffset, size_t numBytes);
		void CopySubresource(resources::GpuResource& dest, u32 destSubIndex, resources::GpuResource& src, u32 srcSubIndex);
		/*
		TODO:
		void CopyCounter()
		void ResetCounter()
		*/

		DynAlloc ReserveUploadMemory(size_t sizeInBytes)
		{
			return _cpuLinearAllocator.Allocate(sizeInBytes);
		}

		static void InitializeTexture(resources::GpuResource& dest, u32 numSubResources, D3D12_SUBRESOURCE_DATA* data);
		static void InitializeTextureArraySlice(resources::GpuResource& dest, u64 sliceIndex, resources::GpuResource& src);
		static void ReadbackTexture2D(resources::GpuResource& readbackBuffer, resources::PixelBuffer& srcBuffer);
		static void InitializeBuffer(resources::GpuResource& dest, const void* data, size_t numBytes, size_t offset = 0);

		void WriteBuffer(resources::GpuResource& dest, size_t destOffset, const void* data, size_t numBytes);
		void FillBuffer(resources::GpuResource& dest, size_t destOffset, float value, size_t numBytes);

		void TransitionResource(resources::GpuResource& dest, D3D12_RESOURCE_STATES newState, bool bFlushImmediate = false);
		void BeginResourceTransition(resources::GpuResource& dest, D3D12_RESOURCE_STATES newState, bool bFlushImmediate = false);
		inline void FlushResourceBarriers();

		void SetPipelineState(PipelineState& pso)
		{
			auto newPSO = pso.GetPSO();

			if (_currentPipelineState == newPSO)
			{
				_commandList->SetPipelineState(_currentPipelineState);
				return;
			}

			_commandList->SetPipelineState(newPSO);
			_currentPipelineState = newPSO;
		}

		inline void SetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type, ID3D12DescriptorHeap* heap)
		{
			if (heap != nullptr && _currentDescriptorHeaps[type] != heap)
			{
				_currentDescriptorHeaps[type] = heap;
				BindDescriptorHeaps();
			}
		}

		inline void SetDescriptorHeaps(size_t numHeaps, D3D12_DESCRIPTOR_HEAP_TYPE* types, ID3D12DescriptorHeap** heaps)
		{
			bool bAnyChanged = false;

			for (size_t i = 0; i < numHeaps; ++i)
			{
				if (heaps[i] != nullptr && heaps[i] != _currentDescriptorHeaps[types[i]])
				{
					_currentDescriptorHeaps[types[i]] = heaps[i];
					bAnyChanged = true;
				}
			}

			if (bAnyChanged)
				BindDescriptorHeaps();
		}

	protected:
		D3D12_COMMAND_LIST_TYPE _type;
		ID3D12GraphicsCommandList* _commandList;
		ID3D12CommandAllocator* _commandAllocator;
		ID3D12PipelineState* _currentPipelineState;
		ID3D12RootSignature* _graphicsRootSig;
		ID3D12RootSignature* _computeRootSig;

		LinearAllocator _cpuLinearAllocator;
		LinearAllocator _gpuLinearAllocator;

		D3D12_RESOURCE_BARRIER _barriers[16];
		u32 _numBarriersToFlush;

		ID3D12DescriptorHeap* _currentDescriptorHeaps[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES];

	protected:
		void BindDescriptorHeaps();

	};

	class RAY_RENDERERCORE_API ComputeContext : public CommandContext
	{
	public:
		static ComputeContext& Begin(bool bAsync = false);

	private:


	};

	class RAY_RENDERERCORE_API GraphicsContext : public CommandContext
	{
	public:
		static GraphicsContext& Begin()
		{
			return CommandContext::Begin().GetGraphicsContext();
		}

		void ClearUAV(resources::GpuResource& target) {}
		void ClearUAV(resources::ColorBuffer& target) {}
		void ClearColor(resources::ColorBuffer& target);
		void ClearDepth() {}
		void ClearStencil() {}
		void ClearDepthAndStencil(resources::DepthBuffer& target);

		void SetRootSignature(const RootSignature& rootSig)
		{
			if (rootSig.GetRootSignature() == _graphicsRootSig)
			{
				_commandList->SetGraphicsRootSignature(_graphicsRootSig);
				return;
			}

			_graphicsRootSig = rootSig.GetRootSignature();
			_commandList->SetGraphicsRootSignature(_graphicsRootSig);

			// TODO:
		}

		void SetRenderTargets(u32 numRTV, D3D12_CPU_DESCRIPTOR_HANDLE* rtv);
		void SetRenderTargets(u32 numRtV, D3D12_CPU_DESCRIPTOR_HANDLE* rtv, D3D12_CPU_DESCRIPTOR_HANDLE dsv);
		void SetRenderTarget(D3D12_CPU_DESCRIPTOR_HANDLE rtv) { SetRenderTargets(1, &rtv); }
		void SetRenderTarget(D3D12_CPU_DESCRIPTOR_HANDLE rtv, D3D12_CPU_DESCRIPTOR_HANDLE dsv) { SetRenderTargets(1, &rtv, dsv); }
		void SetDepthStencilView(D3D12_CPU_DESCRIPTOR_HANDLE dsv) { SetRenderTargets(0, nullptr, dsv); }

		void SetViewport(const D3D12_VIEWPORT& viewport);
		void SetViewport(float x, float y, float w, float h, float minDepth = 0.f, float maxDepth = 1.f);
		void SetScissor(const D3D12_RECT& rect);
		void SetScissor(u32 left, u32 top, u32 right, u32 bottom);
		void SetViewportAndScissor(const D3D12_VIEWPORT& viewport, const D3D12_RECT& rect);
		void SetViewportAndScissor(u32 x, u32 y, u32 w, u32 h);
		void SetBlendFactor(float r, float g, float b, float a);
		void SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY primitiveTopology);

		void SetIndexBuffer(const D3D12_INDEX_BUFFER_VIEW& ibView);
		void SetVertexBuffers(u32 startSlot, u32 count, const D3D12_VERTEX_BUFFER_VIEW* vbViews);
		void SetVertexBuffer(u32 startSlot, const D3D12_VERTEX_BUFFER_VIEW& vbView) { SetVertexBuffers(startSlot, 1, &vbView); }
		void SetDynamicVB(u32 startSlot, size_t numVertices, size_t vertexStride, const void* data);
		void SetDynamicIB(size_t indexCount, const u32* data, bool b32Bit = false);
		void SetDynamicSRV() {}

		void Draw(u32 vertexCount, u32 vertexStartOffset = 0);
		void DrawIndexed(u32 indexCount, u32 startIndexLocation = 0, s32 baseVertexLocation = 0);
		void DrawInstanced(u32 vertexCountPerInstance, u32 instanceCount, u32 startVertexLocation = 0, u32 startInstanceLocation = 0);
		void DrawIndexedInstanced(u32 indexCountPerInstance, u32 instanceCount, u32 startIndexLocation, s32 baseVertexLocation, u32 startInstanceLocation);
		// TODO: 

	private:


	};

}


