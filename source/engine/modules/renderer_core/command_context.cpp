#include "command_context.hpp"
#include <cassert>

namespace ray::renderer_core_api
{
	// ------------------------ CONTEXT MANAGER ------------------------ //

	CommandContext* ContextManager::AllocateContext(CommandListType type)
	{
		_sContextManagerMutex.TryEnter();

		auto& availableContexts = _sAvailableContexts[static_cast<u32>(type)];

		CommandContext* ret = nullptr;

		if (availableContexts.empty())
		{
			ret = new CommandContext(type);
			ret->Initialize();
			_sContextPool[static_cast<u32>(type)].push_back(ret);
		}
		else
		{
			ret = availableContexts.front();
			ret->Reset();
			availableContexts.pop();
		}

		assert(ret != nullptr);
		assert(ret->_type == type);

		_sContextManagerMutex.Leave();
		return ret;
	}

	void ContextManager::FreeContext(CommandContext* context)
	{
		_sContextManagerMutex.TryEnter();

		assert(context != nullptr);
		_sAvailableContexts[static_cast<u32>(context->_type)].push(context);

		_sContextManagerMutex.Leave();
	}

	void ContextManager::DestroyAllContexts()
	{
		for (u16 i = 0; i < 4; i++)
			_sContextPool[i].clear();
	}

	extern ContextManager gContextManager;

	// ------------------------ COMMAND CONTEXT ------------------------ //

	CommandContext& CommandContext::Begin()
	{
		CommandContext* newContext = gContextManager.AllocateContext(CommandListType::eDirect);
		return *newContext;
	}

	u64 CommandContext::Flush(bool bWaitForComplition)
	{
		// FlushResourceBarriers

		u64 fenceValue = gCommandManager.GetQueue(_type).ExecuteCommandList(_commandList);

		if (bWaitForComplition)
			gCommandManager.WaitForFence(fenceValue);

		_commandList->Reset(_commandAllocator, nullptr);

		// TODO: Set Pipeline state

		return fenceValue;
	}

	u64 CommandContext::Finish(bool bWaitForComplition)
	{
		// FlushResourceBarriers

		CommandQueue& queue = gCommandManager.GetQueue(_type);
		u64 fenceValue = queue.ExecuteCommandList(_commandList);
		queue.DiscardAllocator(fenceValue, _commandAllocator);
		_commandAllocator = nullptr;

		_cpuLinearAllocator.CleanupUsedPages(fenceValue);
		_gpuLinearAllocator.CleanupUsedPages(fenceValue);
		// TODO: 

		if (bWaitForComplition)
			gCommandManager.WaitForFence(fenceValue);

		gContextManager.FreeContext(this);

		return fenceValue;
	}

	CommandContext::CommandContext(CommandListType type)
		: _type(type)
		, _cpuLinearAllocator(memory::LinearAllocatorType::eCpuWritable)
		, _gpuLinearAllocator(memory::LinearAllocatorType::eGpuExclusive)
		, _listManager(nullptr)
		, // TODO: FIX GLOBAL VARIABLES
	{}

	// ------------------------ COMPUTE CONTEXT ------------------------ //

	ComputeContext& ComputeContext::Begin(bool bAsync)
	{
		ComputeContext& newContext = gContextManager.AllocateContext
					(bAsync ? CommandListType::eCompute : CommandListType::eDirect)->GetComputeContext();

		return newContext;
	}
}
