#pragma once
#include <core/threading/critical_section.hpp>
#include <core/lib/array.hpp>
#include <queue>
#include <d3d12.h>


class CommandAllocatorPool
{
public:
	CommandAllocatorPool(D3D12_COMMAND_LIST_TYPE type) : _type(type) {}
	~CommandAllocatorPool();

	void Shutdown();

	size_t Size() { return _allocatorPool.size(); }

	ID3D12CommandAllocator* RequestAllocator(u64 completedFenceValue);
	void DiscardAllocator(ID3D12CommandAllocator* allocator, u64 fenceValue);

private:
	D3D12_COMMAND_LIST_TYPE _type;
	CriticalSection _allocatorMutex;
	Array<ID3D12CommandAllocator*> _allocatorPool;
	std::queue<std::pair<u64, ID3D12CommandAllocator*>> _readyAllocators;

};

