#pragma once
#include "command_list.hpp"
#include "command_allocator.hpp"

#include <core/threading/critical_section.hpp>
#include <algorithm>

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
	CommandQueue(IRRCClassHelper* classHelper, CommandListType type);
	~CommandQueue();

	bool Create(IDevice* device);
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
	IFence* _fence;
	IFenceEvent* _event;

	ray::CriticalSection _fenceMutex;
	ray::CriticalSection _eventMutex;

	ICommandQueue* _commandQueue;
	CommandListType _type;
	CommandAllocatorPool _allocatorPool;

	IDevice* _device;
	IRRCClassHelper* _classHelper;

private:
	ICommandAllocator* RequestAllocator();
	void DiscardAllocator(u64 fenceValueForReset, ICommandAllocator* allocator);
	u64 ExecuteCommandList(ICommandList* commandList);

};

class CommandListManager
{
public:
	CommandListManager(IRRCClassHelper* classHelper);
	~CommandListManager();

	void Create(IDevice* device);
	void Shutdown();

	CommandQueue& GetGraphicsQueue() { return _graphicsQueue; }
	CommandQueue& GetComputeQueue() { return _computeQueue; }
	CommandQueue& GetCopyQueue() { return _copyQueue; }

	CommandQueue& GetQueue(CommandListType type)
	{
		switch (type)
		{
		case CommandListType::eCompute:
			return _computeQueue;
		case CommandListType::eCopy:
			return _copyQueue;
		default:
			return _graphicsQueue;
		}
	}

	ICommandQueue* GetCommandQueue() { return _graphicsQueue._commandQueue; }
	void CreateNewCommandList(CommandListType type, ICommandAllocator* allocator, ICommandList* list);
	bool IsFenceComplete(u64 fenceValue)
	{
		return GetQueue(static_cast<CommandListType>(fenceValue >> FENCE_SHIFT)).IsFenceComplete(fenceValue);
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

	IDevice* _device;
	IRRCClassHelper* _classHelper;

};

}