#pragma once
#include "command_allocator.hpp"

#include <core/threading/critical_section.hpp>
#include <d3d12.h>

#define FENCE_SHIFT 56

/*
**	Vulkan:			VkQueue
**	Direct3D 12:	ID3D12CommandQueue
*/

namespace ray::renderer_core_api
{

class CommandListManager;

class CommandQueue
{
	friend class CommandListManager;
	friend class CommandContext;

public:
	CommandQueue(D3D12_COMMAND_LIST_TYPE type);
	~CommandQueue();

	bool Create(ID3D12Device* device);
	void Shutdown();

	bool IsReady() { return _commandQueue != nullptr; }

	u64 IncrementFence();
	void StallForFence(u64 fenceValue);
	void StallForProducer(CommandQueue& producer);
	bool IsFenceComplete(u64 fenceValue);
	void WaitForFence(u64 fenceValue);
	void WaitForIdle() { WaitForFence(IncrementFence()); }

private:
	u64 _nextFenceValue;
	u64 _lastCompletedFenceValue;
	ID3D12Fence* _fence;
	HANDLE _event;

	CriticalSection _fenceMutex;
	CriticalSection _eventMutex;

	ID3D12CommandQueue* _commandQueue;
	D3D12_COMMAND_LIST_TYPE _type;
	CommandAllocatorPool _allocatorPool;

	ID3D12Device* _device;

private:
	ID3D12CommandAllocator* RequestAllocator();
	void DiscardAllocator(u64 fenceValueForReset, ID3D12CommandAllocator* allocator);
	u64 ExecuteCommandList(ID3D12CommandList* commandList);

};

class CommandListManager
{
public:
	CommandListManager();
	~CommandListManager();

	void Create(ID3D12Device* device);
	void Shutdown();

	CommandQueue& GetGraphicsQueue() { return _graphicsQueue; }
	CommandQueue& GetComputeQueue() { return _computeQueue; }
	CommandQueue& GetCopyQueue() { return _copyQueue; }

	CommandQueue& GetQueue(D3D12_COMMAND_LIST_TYPE type)
	{
		switch (type)
		{
		case D3D12_COMMAND_LIST_TYPE_COMPUTE:
			return _computeQueue;
		case D3D12_COMMAND_LIST_TYPE_COPY:
			return _copyQueue;
		default:
			return _graphicsQueue;
		}
	}

	ID3D12CommandQueue* GetCommandQueue() { return _graphicsQueue._commandQueue; }
	void CreateNewCommandList(D3D12_COMMAND_LIST_TYPE type, ID3D12CommandAllocator** allocator, ID3D12GraphicsCommandList** list);
	bool IsFenceComplete(u64 fenceValue)
	{
		return GetQueue(static_cast<D3D12_COMMAND_LIST_TYPE>(fenceValue >> FENCE_SHIFT)).IsFenceComplete(fenceValue);
	}

	void WaitForFence(u64 fenceValue);
	void IdleGPU()
	{
		_graphicsQueue.WaitForIdle();
		_computeQueue.WaitForIdle();
		_copyQueue.WaitForIdle();
	}

private:
	CommandQueue _graphicsQueue;
	CommandQueue _computeQueue;
	CommandQueue _copyQueue;

	ID3D12Device* _device;
	// IRRCClassHelper* _classHelper;

};

}