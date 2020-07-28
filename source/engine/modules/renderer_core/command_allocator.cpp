#include "command_allocator.hpp"
#include "renderer_globals.hpp"

namespace ray::renderer_core_api
{
	CommandAllocatorPool::~CommandAllocatorPool()
	{
		Shutdown();
	}

	ICommandAllocator* CommandAllocatorPool::RequestAllocator(u64 completedFenceValue)
	{
		std::lock_guard<std::mutex> lockGuard(_allocatorMutex);

		ICommandAllocator* ret = gClassHelper->CreateCommandAllocator();

		if (!_readyAllocators.empty())
		{
			std::pair<u64, ICommandAllocator*>& pair = _readyAllocators.front();

			if (pair.first <= completedFenceValue)
			{
				ret = pair.second;
				if (!ret->Reset())
					return nullptr;
				_readyAllocators.pop();
			}
		}

		if (ret->GetInstance() == nullptr)
		{
			if (!gDevice->CreateCommandAllocator(ret, _type))
				return nullptr;
			_allocatorPool.push_back(ret);
		}

		return ret;
	}

	void CommandAllocatorPool::DiscardAllocator(ICommandAllocator* allocator, u64 fenceValue)
	{
		std::lock_guard<std::mutex> lockGuard(_allocatorMutex);

		_readyAllocators.push(std::make_pair(fenceValue, allocator));
	}

	void CommandAllocatorPool::Shutdown()
	{
		for (size_t i = 0; i < _allocatorPool.size(); i++)
			delete _allocatorPool[i];

		_allocatorPool.clear();
	}
}