#include "graphics_memory_manager.hpp"
#include "renderer_core/renderer.hpp"
#include "renderer_core/d3dx12.h"

namespace ray::renderer_core_api::resources
{

	GpuMemoryPool& GpuMemoryManager::RequestPool(u64 requestedSize, u16 heapType, u16 heapResourcesType, u16 resourceType) noexcept
	{
		check(heapType >= HEAP_TYPES_COUNT || heapResourcesType >= HEAP_RESOURCES_TYPES_COUNT, resourceType >= RESOURCE_TYPES_COUNT)
		
		auto poolsCount = _pools[heapType][heapResourcesType][resourceType].Size();
		if (poolsCount <= 0)
			return CreateNewPool(requestedSize, heapType, heapResourcesType, resourceType);

		auto& pool = _pools[heapType][heapResourcesType][resourceType].At(poolsCount - 1);

		if (pool.AvailableSpace < requestedSize)
			return CreateNewPool(requestedSize, heapType, heapResourcesType, resourceType);

		return pool;
	}

	GpuMemoryPool& GpuMemoryManager::CreateNewPool(u64 requestedSize, u16 heapType, u16 heapResourcesType, u16 resourceType) noexcept
	{
		GpuMemoryPool newPool;

		if (resourceType == RESOURCE_TYPE_COMMITTED)
			newPool.Committed.Pool = CreateCommittedPool(requestedSize, heapType, heapResourcesType);
		else if (resourceType == RESOURCE_TYPE_PLACED)
			newPool.Placed.Pool = CreateHeap(requestedSize, heapType, heapResourcesType);

		newPool.AvailableSpace = requestedSize;
		_pools[heapType][heapResourcesType][resourceType].PushBack(newPool);
		auto index = _pools[heapType][heapResourcesType][resourceType].Size() - 1;

		return _pools[heapType][heapResourcesType][resourceType].At(index);
	}

	ID3D12Heap* GpuMemoryManager::CreateHeap(u64 requestedSize, u16 heapType, u16 heapResourcesType) noexcept
	{
		ID3D12Heap* heap;

		D3D12_HEAP_PROPERTIES heapProps = {};
		heapProps.CreationNodeMask = 1;
		heapProps.VisibleNodeMask = 1;
		heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heapProps.Type = D3D12_HEAP_TYPE_DEFAULT;

		D3D12_HEAP_DESC desc = {};
		desc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
		desc.SizeInBytes = requestedSize;
		desc.Properties = heapProps;
		desc.Flags = D3D12_HEAP_FLAG_NONE;

		globals::gDevice->CreateHeap(&desc, IID_PPV_ARGS(&heap));

		return heap;
	}

	ID3D12Resource* GpuMemoryManager::CreateCommittedPool(u64 requestedSize, u16 heapType, u16 heapResourcesType) noexcept
	{
		D3D12_HEAP_PROPERTIES heapProps = {};
		heapProps.CreationNodeMask = 1;
		heapProps.VisibleNodeMask = 1;
		heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heapProps.Type = D3D12_HEAP_TYPE_DEFAULT;

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
		resourceDesc.Width = requestedSize;
		resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

		ID3D12Resource* pool = nullptr;
		globals::gDevice->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&pool));
	}

	void GpuMemoryManager::Destroy() noexcept
	{
		for (size_t i = 0; i < HEAP_TYPES_COUNT; ++i)
			for (size_t j = 0; j < HEAP_RESOURCES_TYPES_COUNT; ++j)
				for (size_t k = 0; k < RESOURCE_TYPES_COUNT; ++k)
					_pools[i][j][k].clear();
	}

}