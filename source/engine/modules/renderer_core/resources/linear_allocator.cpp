#include "linear_allocator.hpp"
#include "../renderer_globals.hpp"
#include "../command_queue.hpp"
#include <core/math/common.hpp>
#include <cassert>

namespace ray::renderer_core_api::memory
{
	LinearAllocatorType LinearAllocatorPageManager::_sDefaultType = LinearAllocatorType::eGpuExclusive;

	LinearAllocatorPageManager::LinearAllocatorPageManager()
	{
		_type = _sDefaultType;
		_sDefaultType = static_cast<LinearAllocatorType>(_sDefaultType + 1);
		assert(_sDefaultType <= eNumAllocatorTypes);
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
		ResourceDesc resourceDesc;
		resourceDesc.ResourceType = ResourceType::eBuffer;
		resourceDesc.Alignment = 0;
		resourceDesc.Height = 1;
		resourceDesc.DepthOrArraySize = 1;
		resourceDesc.MipLevels = 1;
		resourceDesc.ShaderType = ShaderType::eUnknown;
		resourceDesc.SampleDesc.Count = 1;
		resourceDesc.SampleDesc.Quality = 0;
		resourceDesc.TextureLayout = TextureLayout::eRowMajor;

		ResourceState defaultState;
		ResourceHeapProperties heapProps;

		if (_type == eGpuExclusive)
		{
			heapProps.Usage = ResourceUsage::eDefault;
			resourceDesc.Width = pageSize == 0 ? gGpuAllocatorPageSize : pageSize;
			resourceDesc.Flags = ResourceFlags::eAllowUnordererAccess;
			defaultState = ResourceState::eUnorderedAccess;
		}
		else
		{
			heapProps.Usage = ResourceUsage::eUpload;
			resourceDesc.Width = pageSize == 0 ? gCpuAllocatorPageSize : pageSize;
			resourceDesc.Flags = ResourceFlags::eNone;
			defaultState = ResourceState::eGenericRead;
		}

		IResource* buffer = gClassHelper->CreateResource();
		if (!gDevice->CreateCommittedResource(resourceDesc, heapProps, defaultState, buffer))
			return nullptr;

		return new LinearAllocationPage(buffer, defaultState);

	}

	void LinearAllocator::CleanupUsedPages(u64 fenceValue)
	{
		if (_currentPage == nullptr)
			return;

		_retiredPages.push_back(_currentPage);
		_currentPage = nullptr;
		_currentOffset = 0;

		_sPageManager[_type].DiscardPages(fenceValue, _retiredPages);
		_retiredPages.clear();

		_sPageManager[_type].FreeLargePages(fenceValue, _largePages);
		_largePages.clear();
	}

	DynAlloc LinearAllocator::AllocateLargePage(size_t size)
	{
		LinearAllocationPage* page = _sPageManager[_type].CreateNewPage(size);
		_largePages.push_back(page);

		DynAlloc alloc(*page, 0, size);
		alloc.GpuVirtualAddress = page->_gpuVirtualAddress;
		alloc.Data = page->_cpuVirtualAddress;

		return alloc;
	}

	DynAlloc LinearAllocator::Allocate(size_t size, size_t alignment)
	{
		const size_t alignmentMask = alignment - 1;
		assert(alignment & alignmentMask == 0);
		 
		const size_t alignmentSize = ray::core::math::AlignUpWithMask(size, alignmentSize);

		if (alignmentSize > _pageSize)
			return AllocateLargePage(alignmentSize);

		_currentOffset = ray::core::math::AlignUpWithMask(_currentOffset, alignment - 1);

		if (alignmentSize + _currentOffset > _pageSize)
		{
			assert(_currentPage != nullptr);
			_retiredPages.push_back(_currentPage);
			_currentPage = nullptr;
		}

		if (_currentPage == nullptr)
		{
			_currentPage = _sPageManager[_type].RequestPage();
			_currentOffset = 0;
		}

		DynAlloc ret(*_currentPage, _currentOffset, alignmentSize);
		ret.Data = (u8*)_currentPage->_cpuVirtualAddress + _currentOffset;
		ret.GpuVirtualAddress = _currentPage->_gpuVirtualAddress + _currentOffset;

		_currentOffset += alignmentSize;

		return ret;
	}
}