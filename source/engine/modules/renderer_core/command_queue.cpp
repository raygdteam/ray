#include "command_queue.hpp"
#include "renderer_globals.hpp"

namespace ray::renderer_core_api
{
	CommandQueue::CommandQueue(CommandListType type)
		: _type(type)
		, _commandQueue(gClassHelper->CreateCommandQueue())
		, _fence(gClassHelper->CreateFence())
		, _event(gClassHelper->CreateFenceEvent())
		, _nextFenceValue(static_cast<u64>(type) << FENCE_SHIFT | 1)
		, _lastCompletedFenceValue(static_cast<u64>(type) << FENCE_SHIFT)
		, _allocatorPool(type)
	{}

	bool CommandQueue::Create()
	{
		if (IsReady() || _allocatorPool.Size() != 0)
			return false;

		CommandQueueDesc desc;
		desc.Type = _type;
		desc.NodeMask = 1;
		gDevice->CreateCommandQueue(desc, _commandQueue);

		gDevice->CreateFence(_fence, 0);
		_fence->Signal(static_cast<u64>(_type) << FENCE_SHIFT);

		if (!gDevice->CreateFenceEvent(_event, nullptr, false, false))
			return false;

		return IsReady();
	}

	u64 CommandQueue::ExecuteCommandList(ICommandList* list)
	{
		std::lock_guard<std::mutex> lockGuard(_fenceMutex);

		list->Close();

		_commandQueue->SetCommandLists(&list, 1);
		_commandQueue->ExecuteCommandLists();

		_commandQueue->Signal(_fence, _nextFenceValue);

		return _nextFenceValue++;
	}

	u64 CommandQueue::IncrementFence()
	{
		std::lock_guard<std::mutex> lockGuard(_fenceMutex);

		_commandQueue->Signal(_fence, _nextFenceValue);
		
		return _nextFenceValue++;
	}

	bool CommandQueue::IsFenceComplete(u64 fenceValue)
	{
		if (fenceValue > _lastCompletedFenceValue)
			_lastCompletedFenceValue = std::max(_lastCompletedFenceValue, _fence->GetCompletedValue());

		return fenceValue <= _lastCompletedFenceValue;
	}

	void CommandQueue::StallForFence(u64 fenceValue)
	{
		// TODO: 
	}

	void CommandQueue::StallForProducer(CommandQueue& producer)
	{
		if (producer._nextFenceValue <= 0)
			return;

		_commandQueue->Wait(producer._fence, producer._nextFenceValue - 1);
	}

	void CommandQueue::WaitForFence(u64 fenceValue)
	{
		if (IsFenceComplete(fenceValue))
			return;

		{
			std::lock_guard<std::mutex> lockGuard(_eventMutex);

			_fence->SetEventOnCompletion(_event, fenceValue);
			_event->WaitFor();
			_lastCompletedFenceValue = fenceValue;
		}
	}

	ICommandAllocator* CommandQueue::RequestAllocator()
	{
		u64 completedValue = _fence->GetCompletedValue();
		return _allocatorPool.RequestAllocator(completedValue);
	}

	void CommandQueue::DiscardAllocator(u64 fenceValue, ICommandAllocator* allocator)
	{
		_allocatorPool.DiscardAllocator(allocator, fenceValue);
	}

	CommandQueue::~CommandQueue()
	{
		Shutdown();
	}

	void CommandQueue::Shutdown()
	{
		if (_commandQueue->GetInstance() == nullptr)
			return;

		_allocatorPool.Shutdown();

		delete _fence;
		delete _event;
		delete _commandQueue;
	}

	//----------------------------------COMMAND LIST MANAGER----------------------------------//

	extern CommandListManager gCommandManager;

	CommandListManager::CommandListManager()
		: _graphicsQueue(CommandListType::eDirect)
		, _computeQueue(CommandListType::eCompute)
		, _copyQueue(CommandListType::eCopy)
	{}

	void CommandListManager::Create()
	{
		_graphicsQueue.Create();
		_computeQueue.Create();
		_copyQueue.Create();
	}

	void CommandListManager::CreateNewCommandList(CommandListType type, ICommandAllocator* allocator, ICommandList* list)
	{
		switch (type)
		{
		case ray::renderer_core_api::CommandListType::eDirect:
			allocator = _graphicsQueue.RequestAllocator();
			break;
		
		case ray::renderer_core_api::CommandListType::eBundle:
			return;
		
		case ray::renderer_core_api::CommandListType::eCompute:
			allocator = _computeQueue.RequestAllocator();
			break;
		
		case ray::renderer_core_api::CommandListType::eCopy:
			allocator = _copyQueue.RequestAllocator();
			break;
		}

		gDevice->CreateCommandList(list, allocator, nullptr, type);
	}

	void CommandListManager::WaitForFence(u64 fenceValue)
	{
		CommandQueue& producer = GetQueue(static_cast<CommandListType>(fenceValue >> FENCE_SHIFT));
		producer.WaitForFence(fenceValue);
	}

	CommandListManager::~CommandListManager()
	{
		Shutdown();
	}
	
	void CommandListManager::Shutdown()
	{
		_graphicsQueue.Shutdown();
		_computeQueue.Shutdown();
		_copyQueue.Shutdown();
	}
}

