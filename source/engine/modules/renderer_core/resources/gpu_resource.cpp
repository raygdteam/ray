#include "gpu_resource.hpp"
#include <renderer_core/dx12_helper_functions.hpp>
#include <renderer_core/renderer.hpp>
#include <core/log/log.hpp>

template<typename TGpuMemoryPool>
void GpuResourceAllocator<TGpuMemoryPool>::Allocate(GpuResource& resource) noexcept
{
	auto desc = resource.GetDesc();
	D3D12_RESOURCE_DESC resourceDesc = ConvertResourceDescToD3D12_Type(desc);
	resourceDesc.Alignment = 0;

	auto resourceAllocationInfo = gDevice->GetResourceAllocationInfo(1, 1, &resourceDesc);
	resourceDesc.Alignment = resourceAllocationInfo.Alignment;

	if (!_currentPool->IsEnough(resourceAllocationInfo.SizeInBytes))
		_currentPool = _memoryManager.RequestPool(resourceAllocationInfo.SizeInBytes);

	size_t offset = _currentPool->Allocate(resourceAllocationInfo.SizeInBytes);
	auto hr = gDevice->CreatePlacedResource(static_cast<ID3D12Heap*>(_currentPool->GetPool()), offset, &resourceDesc, D3D12_RESOURCE_STATE_COPY_DEST, desc.ClearValue, IID_PPV_ARGS(&resource._resource));

	ray_assert(hr == S_OK, "Could not create gpu resource");

	IRenderer::sRendererStats.UsedGpuMemory += resourceAllocationInfo.SizeInBytes;
	gRendererLogger->Log("Creating GpuResource. Used Gpu Memory: %u KB", IRenderer::sRendererStats.UsedGpuMemory / 1024);

	resource._underlyingPool = _currentPool;
	resource._resourceSize = resourceAllocationInfo.SizeInBytes;
	resource._resourceOffset = offset;
	resource._usageState = D3D12_RESOURCE_STATE_COPY_DEST;
	resource._bManaged = true;
}

template<typename TGpuMemoryPool>
void GpuResourceAllocator<TGpuMemoryPool>::Free(GpuResource& resource) noexcept
{
	u32 refCount = resource._resource->Release();
	gRendererLogger->Log("Releasing GpuResource. Name: %s Reference count: %u", resource._debugName.c_str(), refCount);
	if (resource.IsManaged())
	{
		resource._underlyingPool->Free(resource._resourceOffset, resource._resourceSize);
		IRenderer::sRendererStats.UsedGpuMemory -= resource._resourceSize;

		gRendererLogger->Log("Releasing GpuResource. Used Gpu Memory: %u KB", IRenderer::sRendererStats.UsedGpuMemory / 1024);
	}
}

template<typename TGpuMemoryPool>
void GpuResource::Create(GpuResourceAllocator<TGpuMemoryPool>& allocator, GpuResourceDescription& desc, pcstr debugName) noexcept
{
	_desc = desc;
	allocator.Allocate(*this);

#if defined(RAY_DEBUG) || defined(RAY_DEVELOPMENT)
	if (debugName == nullptr)
		return;

	size_t debugNameSize = strlen(debugName);
	WCHAR dest[128];
	MultiByteToWideChar(0, 0, debugName, debugNameSize, dest, debugNameSize);
	dest[debugNameSize] = '\0';
	_resource->SetName(dest);
	_debugName = String(debugName);
#endif
}
