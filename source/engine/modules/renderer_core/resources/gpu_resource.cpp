#include "gpu_resource.hpp"
#include "../dx12_helper_functions.hpp"

template<typename TGpuMemoryPool>
NODISCARD GpuResource&& GpuResourceAllocator<TGpuMemoryPool>::Allocate(GpuResourceDescription& desc) noexcept
{
	D3D12_RESOURCE_DESC resourceDesc = ConvertResourceDescToD3D12_Type(desc);
	resourceDesc.Alignment = 0;

	auto resourceAllocationInfo = gDevice->GetResourceAllocationInfo(1, 1, &resourceDesc);
	resourceDesc.Alignment = resourceAllocationInfo.Alignment;

	if (!_currentPool->IsEnough(resourceAllocationInfo.SizeInBytes))
		_currentPool = &_memoryManager.RequestPool(resourceAllocationInfo.SizeInBytes);

	ID3D12Resource* resource;
	auto hr = gDevice->CreatePlacedResource(_currentPool->_heap, _currentPool->_offset, &resourceDesc, D3D12_RESOURCE_STATE_COPY_DEST, desc.ClearValue, IID_PPV_ARGS(&resource));

	ray_assert(hr == S_OK, "Could not create gpu resource");

	// TODO: Memory Segment
	_currentPool->_offset += resourceAllocationInfo.SizeInBytes;
	_currentPool->_availableSize = _currentPool->_maxMemoryPoolSize - _currentPool->_offset;

	GpuResource ret;
	ret._desc = std::move(desc);
	ret._underlyingPool = _currentPool;
	ret._resource = resource;
	ret._resourceSize = resourceAllocationInfo.SizeInBytes;
	ret._usageState = D3D12_RESOURCE_STATE_COPY_DEST;
	ret._bManaged = true;

	return std::move(ret);
}

template<typename TGpuMemoryPool>
void GpuResourceAllocator<TGpuMemoryPool>::Free(GpuResource& resource) noexcept
{
	resource._resource->Release();
	if (resource.IsManaged())
	{
		resource._underlyingPool->_availableSize += resource._resourceSize;
	}
	// TODO: MemorySegment
}

template<typename TGpuMemoryPool>
void GpuResource::Create(GpuResourceAllocator<TGpuMemoryPool>& allocator, GpuResourceDescription& desc, pcstr debugName) noexcept
{
	*this = std::move(allocator.Allocate(desc));

#if defined(RAY_DEBUG) || defined(RAY_DEVELOPMENT)
	if (debugName == nullptr)
		return;

	size_t debugNameSize = strlen(debugName);
	WCHAR dest[128];
	MultiByteToWideChar(0, 0, debugName, debugNameSize, dest, debugNameSize);
	dest[debugNameSize] = '\0';
	_resource->SetName(dest);
#endif
}
