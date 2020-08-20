#include "command_allocator.hpp"
#include "renderer_globals.hpp"
#include <cassert>
#include "renderer.hpp"

namespace ray::renderer_core_api
{
	CommandAllocatorPool::~CommandAllocatorPool()
	{
		Shutdown();
	}

	ID3D12CommandAllocator* CommandAllocatorPool::RequestAllocator(u64 completedFenceValue)
	{
		_allocatorMutex.TryEnter();

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
			if (globals::gDevice->CreateCommandAllocator(_type, IID_PPV_ARGS(&ret)) != S_OK)
				return nullptr;
			_allocatorPool.push_back(ret);
		}

		_allocatorMutex.Leave();

		return ret;
	}

	void CommandAllocatorPool::DiscardAllocator(ID3D12CommandAllocator* allocator, u64 fenceValue)
	{
		_allocatorMutex.TryEnter();

		_readyAllocators.push(std::make_pair(fenceValue, allocator));
	
		_allocatorMutex.Leave();
	}

	void CommandAllocatorPool::Shutdown()
	{
		for (size_t i = 0; i < _allocatorPool.size(); i++)
			delete _allocatorPool[i];

		_allocatorPool.clear();
	}
}