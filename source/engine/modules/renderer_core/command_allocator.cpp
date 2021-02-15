#include "command_allocator.hpp"
#include <cassert>
#include "renderer.hpp"

CommandAllocatorPool::~CommandAllocatorPool()
{
	Shutdown();
}

ID3D12CommandAllocator* CommandAllocatorPool::RequestAllocator(u64 completedFenceValue)
{
	_allocatorMutex.Enter();

	ID3D12CommandAllocator* ret = nullptr;

	if (!_readyAllocators.empty())
	{
		std::pair<u64, ID3D12CommandAllocator*>& pair = _readyAllocators.front();

		if (pair.first <= completedFenceValue)
		{
			ret = pair.second;
			if (ret->Reset() != S_OK)
				return nullptr;
			_readyAllocators.pop();
		}
	}

	if (ret == nullptr)
	{
		if (gDevice->CreateCommandAllocator(_type, IID_PPV_ARGS(&ret)) != S_OK)
			return nullptr;
		_allocatorPool.PushBack(ret);
	}

	_allocatorMutex.Leave();

	return ret;
}

void CommandAllocatorPool::DiscardAllocator(ID3D12CommandAllocator* allocator, u64 fenceValue)
{
	_allocatorMutex.Enter();

	_readyAllocators.push(std::make_pair(fenceValue, allocator));

	_allocatorMutex.Leave();
}

void CommandAllocatorPool::Shutdown()
{
	for (size_t i = 0; i < _allocatorPool.Size(); i++)
		_allocatorPool.At(i)->Release();

	_allocatorPool.Clear();
}
