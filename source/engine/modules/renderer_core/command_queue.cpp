#include "command_queue.hpp"
#include <cassert>
#include <algorithm>

namespace ray::renderer_core_api
{
	CommandQueue::CommandQueue(D3D12_COMMAND_LIST_TYPE type)
		: _type(type)
		, _commandQueue(nullptr)
		, _fence(nullptr)
		, _nextFenceValue(static_cast<u64>(type) << FENCE_SHIFT | 1)
		, _lastCompletedFenceValue(static_cast<u64>(type) << FENCE_SHIFT)
		, _allocatorPool(type)
	{}

	bool CommandQueue::Create(ID3D12Device* device)
	{
		if (IsReady() || _allocatorPool.Size() != 0)
			return false;

		_device = device;

		D3D12_COMMAND_QUEUE_DESC desc;
		desc.Type = _type;
		desc.NodeMask = 1;
		_device->CreateCommandQueue(&desc, IID_PPV_ARGS(&_commandQueue));

		_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence));
		_fence->Signal(static_cast<u64>(_type) << FENCE_SHIFT);

		_event = CreateEvent(nullptr, false, false, nullptr);
		assert(_event != nullptr);

		return IsReady();
	}

	u64 CommandQueue::ExecuteCommandList(ID3D12CommandList* list)
	{
		_fenceMutex.Enter();

		static_cast<ID3D12GraphicsCommandList*>(list)->Close();

		_commandQueue->ExecuteCommandLists(1, &list);

		_commandQueue->Signal(_fence, _nextFenceValue);

		_fenceMutex.Leave();

		return _nextFenceValue++;
	}

	u64 CommandQueue::IncrementFence()
	{
		_fenceMutex.Enter();

		_commandQueue->Signal(_fence, _nextFenceValue);
		
		_fenceMutex.Leave();

		return _nextFenceValue++;
	}

	bool CommandQueue::IsFenceComplete(u64 fenceValue)
	{
		using namespace std;
		if (fenceValue > _lastCompletedFenceValue)
			_lastCompletedFenceValue = max(_lastCompletedFenceValue, _fence->GetCompletedValue());

		return fenceValue <= _lastCompletedFenceValue;
	}

	namespace globals
	{
		extern CommandListManager gCommandListManager;
	}

	void CommandQueue::StallForFence(u64 fenceValue)
	{
		CommandQueue& producer = globals::gCommandListManager.GetQueue(static_cast<D3D12_COMMAND_LIST_TYPE>(fenceValue >> FENCE_SHIFT));
		_commandQueue->Wait(producer._fence, fenceValue);
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

		// TODO:  Think about how this might affect a multi-threaded situation.  Suppose thread A
		// wants to wait for fence 100, then thread B comes along and wants to wait for 99.  If
		// the fence can only have one event set on completion, then thread B has to wait for 
		// 100 before it knows 99 is ready.  Maybe insert sequential events?
		{
			_eventMutex.Enter();

			_fence->SetEventOnCompletion(fenceValue, _event);
			WaitForSingleObject(_event, INFINITE);
			_lastCompletedFenceValue = fenceValue;
		
			_eventMutex.Leave();
		}
	}

	ID3D12CommandAllocator* CommandQueue::RequestAllocator()
	{
		u64 completedValue = _fence->GetCompletedValue();
		return _allocatorPool.RequestAllocator(completedValue);
	}

	void CommandQueue::DiscardAllocator(u64 fenceValue, ID3D12CommandAllocator* allocator)
	{
		_allocatorPool.DiscardAllocator(allocator, fenceValue);
	}

	CommandQueue::~CommandQueue()
	{
		Shutdown();
	}

	void CommandQueue::Shutdown()
	{
		if (_commandQueue == nullptr)
			return;

		_allocatorPool.Shutdown();

		_fence->Release();
		_fence = nullptr;

		CloseHandle(_event);
		_event = nullptr;

		_commandQueue->Release();
		_commandQueue = nullptr;
	}

	//----------------------------------COMMAND LIST MANAGER----------------------------------//


	CommandListManager::CommandListManager()
		: _graphicsQueue(D3D12_COMMAND_LIST_TYPE_DIRECT)
		, _computeQueue(D3D12_COMMAND_LIST_TYPE_COMPUTE)
		, _copyQueue(D3D12_COMMAND_LIST_TYPE_COPY)
	{}

	void CommandListManager::Create(ID3D12Device* device)
	{
		_device = device;

		_graphicsQueue.Create(device);
		_computeQueue.Create(device);
		_copyQueue.Create(device);
	}

	void CommandListManager::CreateNewCommandList(D3D12_COMMAND_LIST_TYPE type, ID3D12CommandAllocator* allocator, ID3D12CommandList* list)
	{
		switch (type)
		{
		case D3D12_COMMAND_LIST_TYPE_DIRECT:
			allocator = _graphicsQueue.RequestAllocator();
			break;
		
		case D3D12_COMMAND_LIST_TYPE_BUNDLE:
			return;
		
		case D3D12_COMMAND_LIST_TYPE_COMPUTE:
			allocator = _computeQueue.RequestAllocator();
			break;
		
		case D3D12_COMMAND_LIST_TYPE_COPY:
			allocator = _copyQueue.RequestAllocator();
			break;
		default:
			allocator = _graphicsQueue.RequestAllocator();

		}

		assert(_device->CreateCommandList(1, type, allocator, nullptr, IID_PPV_ARGS(&list)) == S_OK);
	}

	void CommandListManager::WaitForFence(u64 fenceValue)
	{
		CommandQueue& producer = GetQueue(static_cast<D3D12_COMMAND_LIST_TYPE>(fenceValue >> FENCE_SHIFT));
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

