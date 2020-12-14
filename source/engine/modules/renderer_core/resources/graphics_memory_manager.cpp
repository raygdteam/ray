#include "graphics_memory_manager.hpp"
#include "renderer_core/renderer.hpp"
#include "renderer_core/d3dx12.h"
#include <core/debug/assert.hpp>
#include <core/math/common.hpp>
#include "../dx12_helper_functions.hpp"

using namespace ray;
using namespace ray::core;

namespace ray::renderer_core_api::resources
{

	GpuMemoryPool& GpuMemoryManager::RequestPool(u64 requestedSize, u16 heapType, u16 dimensionType, u16 resourceType) noexcept
	{
		check(heapType < HEAP_TYPES_COUNT&& dimensionType < RESOURCE_DIMENSIONS_COUNT&& resourceType < RESOURCE_TYPES_COUNT)

		_mutex.Enter();
		size_t poolsCount = _pools[heapType][dimensionType][resourceType].Size();
		if (poolsCount <= 0)
			return CreateNewPool(requestedSize, heapType, dimensionType, resourceType);

		auto& pool = _pools[heapType][dimensionType][resourceType].At(poolsCount - 1);

		if (pool.AvailableSpace < requestedSize)
			return CreateNewPool(requestedSize, heapType, dimensionType, resourceType);
		_mutex.Leave();

		return pool;
	}

	GpuMemoryPool& GpuMemoryManager::CreateNewPool(u64 requestedSize, u16 heapType, u16 dimensionType, u16 resourceType) noexcept
	{
		GpuMemoryPool newPool;
		u64 heapSize = (_preferencedHeapSize > requestedSize) ? _preferencedHeapSize : requestedSize;

		switch (resourceType)
		{
		case RESOURCE_TYPE_COMMITTED:
			newPool.Committed.Pool = CreateCommittedPool(heapSize);
			break;
		case RESOURCE_TYPE_PLACED:
			newPool.Placed.Pool = CreateHeap(heapSize, heapType);
			break;
		}

		newPool.AvailableSpace = heapSize;
		newPool.MaxSpaceSize = heapSize;
		newPool.Offset = 0;

		_pools[heapType][dimensionType][resourceType].PushBack(newPool);
		size_t index = _pools[heapType][dimensionType][resourceType].Size() - 1;

		return _pools[heapType][dimensionType][resourceType].At(index);
	}

	ID3D12Heap* GpuMemoryManager::CreateHeap(u64 requestedSize, u16 heapType) noexcept
	{
		ID3D12Heap* heap = nullptr;
		u64 alignedSize = math::AlignUp(requestedSize, KB(64));

		D3D12_HEAP_TYPE type;

		switch (heapType)
		{
		case HEAP_TYPE_DEFAULT:
			type = D3D12_HEAP_TYPE_DEFAULT;
			break;

		case HEAP_TYPE_UPLOAD:
			type = D3D12_HEAP_TYPE_UPLOAD;
			break;

		case HEAP_TYPE_READBACK:
			type = D3D12_HEAP_TYPE_READBACK;
			break;
		}

		auto heapProps = dx12::DescribeHeapProps(type);
		auto heapDesc = dx12::DescribeHeap(heapProps, D3D12_HEAP_FLAG_NONE, alignedSize);

		globals::gDevice->CreateHeap(&heapDesc, IID_PPV_ARGS(&heap));

		return heap;
	}

	ID3D12Resource* GpuMemoryManager::CreateCommittedPool(u64 requestedSize) noexcept
	{
		auto heapProps = dx12::DescribeHeapProps(D3D12_HEAP_TYPE_UPLOAD);
		auto resourceDesc = dx12::DescribeBuffer(requestedSize);

		ID3D12Resource* pool = nullptr;
		globals::gDevice->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&pool));
	
		return pool;
	}

	void GpuMemoryManager::Destroy() noexcept
	{
		for (size_t i = 0; i < HEAP_TYPES_COUNT; ++i)
			for (size_t j = 0; j < RESOURCE_DIMENSIONS_COUNT; ++j)
				for (size_t k = 0; k < RESOURCE_TYPES_COUNT; ++k)
					_pools[i][j][k].clear();
	}

}