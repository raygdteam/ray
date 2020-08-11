#pragma once
#include "command_list.hpp"
#include "command_queue.hpp"
#include "command_allocator.hpp"
#include "resources/linear_allocator.hpp"
#include "pipeline_state.hpp"
#include <core/threading/critical_section.hpp>

namespace ray::renderer_core_api
{
	enum class CommandContextType
	{
		e3D,
		eCopy,
		eCompute
		//, eBundle
	};

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

		CommandContext* AllocateContext(CommandListType type) noexcept;
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
		CommandContext(CommandListType type);
		
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

		ICommandList* GetCommandList() const noexcept
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

		memory::DynAlloc ReserveUploadMemory(size_t sizeInBytes)
		{
			return _cpuLinearAllocator.Allocate(sizeInBytes);
		}

		static void InitializeTexture(resources::GpuResource& dest, u32 numSubResources, const void* data, u64 rowPitch, u64 slicePitch)
		{
			//TODO:
		}

		static void InitializeTextureArraySlice(resources::GpuResource& dest, u64 sliceIndex, resources::GpuResource& src)
		{
			//TODO:
		}

		//TODO: void ReadbackTexture2D()
		
		static void InitializeBuffer(resources::GpuResource& dest, const void* data, size_t numBytes, size_t offset = 0);

		void WriteBuffer(resources::GpuResource& dest, size_t destOffset, const void* data, size_t numBytes);
		void FillBuffer(resources::GpuResource& dest, size_t destOffset, float value, size_t numBytes);

		void TransitionResource(resources::GpuResource& dest, resources::ResourceState newState, bool bFlushImmediate = false);
		void BeginResourceTransition(resources::GpuResource& dest, resources::ResourceState newState, bool bFlushImmediate = false);
		inline void FlushResourceBarriers();

		void SetPipelineState(IPipelineState* pso)
		{
			if (_currentPipelineState == pso)
				return;

			_commandList->SetPipelineState(pso);
			_currentPipelineState = pso;
		}

	protected:
		CommandListType _type;
		CommandListManager _listManager;
		ICommandList* _commandList;
		ICommandAllocator* _commandAllocator;
		IPipelineState* _currentPipelineState;

		memory::LinearAllocator _cpuLinearAllocator;
		memory::LinearAllocator _gpuLinearAllocator;

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


