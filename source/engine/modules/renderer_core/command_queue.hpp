#pragma once
#include "command_list.hpp"
#include "fence.hpp"
#include "command_allocator.hpp"

#include <mutex>
#include <algorithm>

#define FENCE_SHIFT 56

/*
**	Vulkan:			VkQueue
**	Direct3D 12:	ID3D12CommandQueue
*/

namespace ray::renderer_core_api
{

struct CommandQueueDesc
{
	CommandListType Type;
	u32 NodeMask;
};

class ICommandQueue : public IRRCBase
{
public:
	virtual ~ICommandQueue() {}

	virtual void SetCommandLists(ICommandList**, size_t) = 0;
	virtual void ExecuteCommandLists() = 0;
	virtual bool Signal(IFence*, u32) = 0;
	virtual bool Wait(IFence* fence, u64 fenceValue) = 0;

};

class CommandListManager;

class CommandQueue
{
	friend CommandListManager;
public:
	CommandQueue(CommandListType type);
	~CommandQueue();

	bool Create();
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

	std::mutex _fenceMutex;
	std::mutex _eventMutex;

	ICommandQueue* _commandQueue;
	CommandListType _type;
	CommandAllocatorPool _allocatorPool;

private:
	ICommandAllocator* RequestAllocator();
	void DiscardAllocator(u64 fenceValueForReset, ICommandAllocator* allocator);
	u64 ExecuteCommandList(ICommandList* commandList);

};

class CommandListManager
{
public:
	CommandListManager();
	~CommandListManager();

	void Create();
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

};

}