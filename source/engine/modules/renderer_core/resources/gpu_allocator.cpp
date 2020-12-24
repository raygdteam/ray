#include "gpu_allocator.hpp"
#include <renderer_core/dx12_helper_functions.hpp>
#include "graphics_memory_manager.hpp"
#include <renderer_core/renderer.hpp>
#include <core/debug/assert.hpp>
#include <core/math/common.hpp>

using namespace ray;
using namespace ray::core;

namespace ray::renderer_core_api::resources
{
	AllocatedResource& GpuAllocator::AllocateBuffer(u64 size, u16 heapType) noexcept
	{
		auto desc = dx12::DescribeBuffer(size);
		return AllocateResource(desc, heapType);
	}

	AllocatedResource& GpuAllocator::AllocateTexture2D(DXGI_FORMAT format, u64 width, u64 height, u16 heapType) noexcept
	{
		auto desc = dx12::DescribeDefaultTexture2D(format, width, height);
		return AllocateResource(desc, heapType);
	}
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreturn-stack-address"
	AllocatedResource& GpuAllocator::AllocateResource(D3D12_RESOURCE_DESC& desc, u16 heapType) noexcept
	{
		u16 resourceDimension = 0;
		switch (desc.Dimension)
		{
		case D3D12_RESOURCE_DIMENSION_BUFFER: 
			resourceDimension = RESOURCE_DIMENSION_BUFFER;
			break;
		case D3D12_RESOURCE_DIMENSION_TEXTURE1D:
		case D3D12_RESOURCE_DIMENSION_TEXTURE2D:
		case D3D12_RESOURCE_DIMENSION_TEXTURE3D:
			resourceDimension = RESOURCE_DIMENSION_TEXTURE;
			break;
		case D3D12_RESOURCE_DIMENSION_UNKNOWN:
			check(false)
		}

		AllocatedResource resource;
		u64 resourceSize = desc.Width * desc.Height;
		auto& pool = globals::gGpuMemManager.RequestPool(resourceSize, heapType, resourceDimension, RESOURCE_TYPE_PLACED);

		globals::gDevice->CreatePlacedResource(pool.Placed.Pool, pool.Offset, &desc, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&resource.Resource));
		
		resource.Heap = pool.Placed.Pool;
		resource.Offset = pool.Offset;

		pool.Offset += math::AlignUp(resourceSize, desc.Alignment);
		pool.AvailableSpace -= pool.Offset;

		// TODO:
		return resource;
	}
	AllocatedResource& GpuAllocator::AllocateUploadHeap(u64 size) noexcept
	{
		auto& pool = globals::gGpuMemManager.RequestPool(size, HEAP_TYPE_UPLOAD, RESOURCE_DIMENSION_BUFFER, RESOURCE_TYPE_COMMITTED);
		
		AllocatedResource resource;
		resource.Heap = nullptr;
		resource.Resource = pool.Committed.Pool;
		resource.Offset = pool.Offset;

		pool.Offset += math::AlignUp(size, KB(64));
		pool.AvailableSpace -= pool.Offset;

		// TODO: 
		return resource;
	}
#pragma clang diagnostic pop
}