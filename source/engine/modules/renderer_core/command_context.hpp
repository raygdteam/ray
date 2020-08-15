#pragma once
#include "command_queue.hpp"
#include "resources/linear_allocator.hpp"
#include "pipeline_state.hpp"
#include <core/threading/critical_section.hpp>
#include <d3d12.h>

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

	class ContextManager
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

		static void InitializeTexture(resources::GpuResource& dest, u32 numSubResources, const void* data, u64 rowPitch, u64 slicePitch);
		static void InitializeTextureArraySlice(resources::GpuResource& dest, u64 sliceIndex, resources::GpuResource& src);
		static void ReadbackTexture2D(resources::GpuResource& readbackBuffer/*, TODO: resources::PixelBuffer& srcBuffer*/);
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
				return;

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

		LinearAllocator _cpuLinearAllocator;
		LinearAllocator _gpuLinearAllocator;

		D3D12_RESOURCE_BARRIER _barriers[16];
		u32 _numBarriersToFlush;

		ID3D12DescriptorHeap* _currentDescriptorHeaps[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES];

	protected:
		void BindDescriptorHeaps();

	};

	class ComputeContext : public CommandContext
	{
	public:
		static ComputeContext& Begin(bool bAsync = false);

	private:


	};

	class GraphicsContext : public CommandContext
	{
	public:


	private:


	};

}


