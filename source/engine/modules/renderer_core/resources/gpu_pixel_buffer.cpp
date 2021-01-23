#include "gpu_pixel_buffer.hpp"
#include <renderer_core/renderer.hpp>
#include <core/debug/assert.hpp>

NODISCARD GpuResource&& GpuPixelBufferAllocator::Allocate(GpuResourceDescription& textureDesc) noexcept
{
	D3D12_RESOURCE_DESC resourceDesc = {};
	resourceDesc.Width = textureDesc.Width;
	resourceDesc.Height = textureDesc.Height;
	resourceDesc.MipLevels = textureDesc.MipLevels;
	resourceDesc.DepthOrArraySize = textureDesc.ArraySize;
	resourceDesc.Dimension = textureDesc.Dimension;
	resourceDesc.Flags = textureDesc.Flags;
	resourceDesc.Format = textureDesc.Format;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	resourceDesc.SampleDesc = textureDesc.SampleDesc;

	auto resourceAllocationInfo = gDevice->GetResourceAllocationInfo(1, 1, &resourceDesc);
	resourceDesc.Alignment = resourceAllocationInfo.Alignment;

	if (!_currentPool->IsEnough(resourceAllocationInfo.SizeInBytes))
		_currentPool = &_memoryManager.RequestPool(resourceAllocationInfo.SizeInBytes);

	ID3D12Resource* resource;
	auto hr = gDevice->CreatePlacedResource(_currentPool->_heap, _currentPool->_offset, &resourceDesc, D3D12_RESOURCE_STATE_COPY_DEST, textureDesc.ClearValue, IID_PPV_ARGS(&resource));

	check(hr == S_OK)

	_currentPool->_offset += resourceAllocationInfo.SizeInBytes;
	_currentPool->_availableSize -= _currentPool->_offset;

	GpuPixelBuffer ret;
	ret._desc = std::move(textureDesc);
	ret._underlyingPool = _currentPool;
	ret._resource = resource;
	ret._resourceSize = resourceAllocationInfo.SizeInBytes;
	ret._usageState = D3D12_RESOURCE_STATE_COPY_DEST;

	return std::move(ret);
}

void GpuPixelBufferAllocator::Free(GpuResource& resource) noexcept
{
	dynamic_cast<GpuPixelBuffer&>(resource)._resource->Release();
	if (resource.IsManaged())
	{
		dynamic_cast<GpuPixelBufferMemoryPool*>(dynamic_cast<GpuPixelBuffer&>(resource)._underlyingPool)->_availableSize += dynamic_cast<GpuPixelBuffer&>(resource)._resourceSize;
	}

	// TODO: MemorySegment
}

void GpuPixelBuffer::Create(GpuTextureDescription& desc) noexcept
{
	ray_assert((desc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE1D  ||
				desc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D  ||
				desc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE3D) &&
			  ((desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET) ||
			   (desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL)),
				"Resource dimension must be texture and resource must be render target or depth stencil buffer!")

	*dynamic_cast<GpuResource*>(this) = std::move(gPixelBufferAllocator.Allocate(desc));
	// TODO
}

void GpuPixelBuffer::Release() noexcept
{
	gPixelBufferAllocator.Free(*this);
}
