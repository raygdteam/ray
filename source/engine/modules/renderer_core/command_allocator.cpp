#include "command_allocator.hpp"
#include "renderer_globals.hpp"
#include <cassert>

namespace ray::renderer_core_api
{
	CommandAllocatorPool::~CommandAllocatorPool()
	{
		Shutdown();
	}

	ICommandAllocator* CommandAllocatorPool::RequestAllocator(u64 completedFenceValue)
	{
		_allocatorMutex.TryEnter();

		ICommandAllocator* ret = gClassHelper->CreateCommandAllocator();

		if (!_readyAllocators.empty())
		{
			std::pair<u64, ICommandAllocator*>& pair = _readyAllocators.front();

			if (pair.first <= completedFenceValue)
			{
				ret = pair.second;
				assert(ret->Reset());
				_readyAllocators.pop();
			}
		}

		if (ret->GetInstance() == nullptr)
		{
			if (!gDevice->CreateCommandAllocator(ret, _type))
				return nullptr;
			_allocatorPool.push_back(ret);
		}

		_allocatorMutex.Leave();

		return ret;
	}

	void CommandAllocatorPool::DiscardAllocator(ICommandAllocator* allocator, u64 fenceValue)
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