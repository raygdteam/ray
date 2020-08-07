#pragma once
#include "command_list.hpp"
#include "command_queue.hpp"
#include "command_allocator.hpp"
#include "resources/linear_allocator.hpp"
#include "pipeline_state.hpp"
#include <core/threading/critical_section.hpp>

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

		ComputeContext& GetComputeContext()
		{
			return reinterpret_cast<ComputeContext&>(*this);
		}

	protected:
		CommandListType _type;
		CommandListManager _listManager;
		ICommandList* _commandList;
		ICommandAllocator* _commandAllocator;
		IPipelineState* _currentPipelineState;

		memory::LinearAllocator _cpuLinearAllocator;
		memory::LinearAllocator _gpuLinearAllocator;
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


