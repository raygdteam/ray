#include "linear_allocator.hpp"
#include "../renderer_globals.hpp"
#include "../command_queue.hpp"

namespace ray::renderer_core_api::memory
{
	LinearAllocatorType LinearAllocatorPageManager::_sDefaultType = LinearAllocatorType::eGpuExclusive;

	LinearAllocatorPageManager::LinearAllocatorPageManager()
	{
		_type = _sDefaultType;
		_sDefaultType = static_cast<LinearAllocatorType>(_sDefaultType + 1);
		// TODO: ASSERT (_sDefaultType <= eNumAllocatorTypes)
	}

	LinearAllocatorPageManager LinearAllocator::_sPageManager[eNumAllocatorTypes];

	LinearAllocationPage* LinearAllocatorPageManager::RequestPage()
	{
		std::lock_guard<std::mutex> lockGuard(_mutex);

		while (!_retiredPage.empty() && gCommandManager.IsFenceComplete(_retiredPage.front().first))
		{
			_availablePages.push(_retiredPage.front().second);
			_retiredPage.pop();
		}

		LinearAllocationPage* ret = nullptr;

		if (!_availablePages.empty())
		{
			ret = _availablePages.front();
			_availablePages.pop();
		}
		else
		{
			ret = CreateNewPage();
			_pagePool.emplace_back(ret);
		}

		return ret;
	}

	void LinearAllocatorPageManager::DiscardPages(u64 fenceValue, const std::vector<LinearAllocationPage*>& pages)
	{
		std::lock_guard<std::mutex> lockGuard(_mutex);
		for (auto it = pages.cbegin(); it != pages.cend(); ++it)
			_retiredPage.push(std::make_pair(fenceValue, *it));
	}

	void LinearAllocatorPageManager::FreeLargePages(u64 fenceValue, const std::vector<LinearAllocationPage*>& pages)
	{
		while (!_deletionQueue.empty() && gCommandManager.IsFenceComplete(_deletionQueue.front().first))
		{
			delete _deletionQueue.front().second;
			_deletionQueue.pop();
		}

		for (auto it = pages.cbegin(); it != pages.cend(); ++it)
		{
			(*it)->Unmap();
			_deletionQueue.push(std::make_pair(fenceValue, *it));
		}
	}

	LinearAllocationPage* LinearAllocatorPageManager::CreateNewPage(size_t pageSize)
	{
		
	}
}