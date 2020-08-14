#include "linear_allocator.hpp"
#include "../renderer.hpp"
#include "../command_queue.hpp"
#include <core/math/common.hpp>
#include <cassert>

namespace ray::renderer_core_api
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
		_mutex.Enter();

		while (!_retiredPage.empty() && globals::gCommandListManager.IsFenceComplete(_retiredPage.front().first))
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

		_mutex.Leave();

		return ret;
	}

	void LinearAllocatorPageManager::DiscardPages(u64 fenceValue, const std::vector<LinearAllocationPage*>& pages)
	{
		_mutex.Enter();
		for (auto it = pages.cbegin(); it != pages.cend(); ++it)
			_retiredPage.push(std::make_pair(fenceValue, *it));
		
		_mutex.Leave();
	}

	void LinearAllocatorPageManager::FreeLargePages(u64 fenceValue, const std::vector<LinearAllocationPage*>& pages)
	{
		while (!_deletionQueue.empty() && globals::gCommandListManager.IsFenceComplete(_deletionQueue.front().first))
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
		D3D12_RESOURCE_DESC resourceDesc;
		resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resourceDesc.Alignment = 0;
		resourceDesc.Height = 1;
		resourceDesc.DepthOrArraySize = 1;
		resourceDesc.MipLevels = 1;
		resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
		resourceDesc.SampleDesc.Count = 1;
		resourceDesc.SampleDesc.Quality = 0;
		resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		D3D12_RESOURCE_STATES defaultState;
		D3D12_HEAP_PROPERTIES heapProps;

		if (_type == eGpuExclusive)
		{
			heapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
			resourceDesc.Width = pageSize == 0 ? gGpuAllocatorPageSize : pageSize;
			resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
			defaultState = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
		}
		else
		{
			heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
			resourceDesc.Width = pageSize == 0 ? gCpuAllocatorPageSize : pageSize;
			resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
			defaultState = D3D12_RESOURCE_STATE_GENERIC_READ;
		}

		ID3D12Resource* buffer = nullptr;
		if (!globals::gDevice->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, defaultState, nullptr, IID_PPV_ARGS(&buffer)))
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