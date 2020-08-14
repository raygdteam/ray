#include "command_context.hpp"
#include <cassert>
#include "renderer.hpp"

// TODO: FIX GLOBAL VARIABLES

namespace ray::renderer_core_api
{
	// ------------------------ CONTEXT MANAGER ------------------------ //

	CommandContext* ContextManager::AllocateContext(D3D12_COMMAND_LIST_TYPE type) noexcept
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

	void ContextManager::FreeContext(CommandContext* context) noexcept
	{
		_sContextManagerMutex.TryEnter();

		assert(context != nullptr);
		_sAvailableContexts[static_cast<u32>(context->_type)].push(context);

		_sContextManagerMutex.Leave();
	}

	void ContextManager::DestroyAllContexts() noexcept
	{
		for (u16 i = 0; i < 4; i++)
			_sContextPool[i].clear();
	}

	// ------------------------ COMMAND CONTEXT ------------------------ //

	CommandContext& CommandContext::Begin()
	{
		CommandContext* newContext = globals::gContextManager.AllocateContext(D3D12_COMMAND_LIST_TYPE_DIRECT);
		return *newContext;
	}

	u64 CommandContext::Flush(bool bWaitForComplition)
	{
		// FlushResourceBarriers

		u64 fenceValue = globals::gCommandListManager.GetQueue(_type).ExecuteCommandList(_commandList);

		if (bWaitForComplition)
			globals::gCommandListManager.WaitForFence(fenceValue);

		_commandList->Reset(_commandAllocator, nullptr);

		// TODO: Set Pipeline state

		return fenceValue;
	}

	u64 CommandContext::Finish(bool bWaitForComplition)
	{
		// FlushResourceBarriers

		CommandQueue& queue = globals::gCommandListManager.GetQueue(_type);
		u64 fenceValue = queue.ExecuteCommandList(_commandList);
		queue.DiscardAllocator(fenceValue, _commandAllocator);
		_commandAllocator = nullptr;

		_cpuLinearAllocator.CleanupUsedPages(fenceValue);
		_gpuLinearAllocator.CleanupUsedPages(fenceValue);
		// TODO: 

		if (bWaitForComplition)
			globals::gCommandListManager.WaitForFence(fenceValue);

		globals::gContextManager.FreeContext(this);

		return fenceValue;
	}

	CommandContext::CommandContext(D3D12_COMMAND_LIST_TYPE type)
		: _type(type)
		, _cpuLinearAllocator(LinearAllocatorType::eCpuWritable)
		, _gpuLinearAllocator(LinearAllocatorType::eGpuExclusive)
		, _listManager()
		, _commandList(nullptr)
		, _commandAllocator(nullptr)
		, _numBarriersToFlush(0)
		//TODO: 
	{}

	CommandContext::~CommandContext()
	{
		if (_commandList != nullptr)
		{
			_commandList->Release();
			_commandList = nullptr;
		}

		if (_commandAllocator != nullptr)
		{
			_commandAllocator->Release();
			_commandAllocator = nullptr;
		}
	}
	
	void CommandContext::Initialize()
	{
		globals::gCommandListManager.CreateNewCommandList(_type, _commandAllocator, _commandList);
	}

	void CommandContext::Reset()
	{
		assert(_commandList != nullptr && _commandAllocator == nullptr);
		_commandAllocator = globals::gCommandListManager.GetQueue(_type).RequestAllocator();
		_commandList->Reset(_commandAllocator, nullptr);

		//TODO:
	}

	void CommandContext::TransitionResource(resources::GpuResource& dest, D3D12_RESOURCE_STATES newState, bool bFlushImmediate)
	{
		D3D12_RESOURCE_STATES oldState = dest._usageState;
		if (newState == oldState)
		{
			assert(_numBarriersToFlush < 16);
			D3D12_RESOURCE_BARRIER& barrier = _barriers[_numBarriersToFlush++];
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Transition.StateAfter = newState;
			barrier.Transition.StateBefore = oldState;
			barrier.Transition.pResource = dest._resource;
			barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

			if (newState == dest._transitioningState)
			{
				barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_END_ONLY;
				dest._transitioningState = static_cast<D3D12_RESOURCE_STATES>(-1);
			}
			else
				barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

			dest._usageState = newState;
		}
		else if(newState == D3D12_RESOURCE_STATE_UNORDERED_ACCESS)
		{ /*TODO:*/ }

		if (bFlushImmediate || _numBarriersToFlush == 16)
			FlushResourceBarriers();
	}

	void CommandContext::BeginResourceTransition(resources::GpuResource& dest, D3D12_RESOURCE_STATES newState, bool bFlushImmediate)
	{
		if (dest._transitioningState != static_cast<D3D12_RESOURCE_STATES>(-1))
			TransitionResource(dest, dest._transitioningState);

		auto oldState = dest._usageState;
		if (oldState != newState)
		{
			assert(_numBarriersToFlush < 16);
			auto& barrier = _barriers[_numBarriersToFlush++];

			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Transition.pResource = dest._resource;
			barrier.Transition.StateAfter = newState;
			barrier.Transition.StateBefore = oldState;
			barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_BEGIN_ONLY;

			dest._transitioningState = newState;
		}

		if (bFlushImmediate || _numBarriersToFlush == 16)
			FlushResourceBarriers();
	}

	void CommandContext::FlushResourceBarriers()
	{
		if (_numBarriersToFlush > 0)
		{
			_commandList->ResourceBarrier(_numBarriersToFlush, _barriers);
			_numBarriersToFlush = 0;
		}
	}

	void CommandContext::BindDescriptorHeaps()
	{
		
	}

	// ------------------------ COMPUTE CONTEXT ------------------------ //

	ComputeContext& ComputeContext::Begin(bool bAsync)
	{
		ComputeContext& newContext = globals::gContextManager.AllocateContext
					(bAsync ? D3D12_COMMAND_LIST_TYPE_COMPUTE : D3D12_COMMAND_LIST_TYPE_DIRECT)->GetComputeContext();

		return newContext;
	}
}
