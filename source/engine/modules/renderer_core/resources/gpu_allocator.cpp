#include "gpu_allocator.hpp"
#include <renderer_core/dx12_helper_functions.hpp>
#include "graphics_memory_manager.hpp"
#include <renderer_core/renderer.hpp>
#include <core/debug/assert.hpp>
#include <core/math/common.hpp>
#include <renderer_core/resources/resources_table.hpp>

using namespace ray;
using namespace ray::core;

namespace ray::renderer_core_api::resources
{
	AllocatedResource& GpuAllocator::AllocateBuffer(u64 localId, u64 size, u16 heapType) noexcept
	{
		auto desc = dx12::DescribeBuffer(size);
		return AllocateResource(desc, localId, heapType);
	}

	AllocatedResource& GpuAllocator::AllocateTexture2D(u64 localId, u64 width, u64 height, DXGI_FORMAT format, u16 heapType) noexcept
	{
		auto desc = dx12::DescribeDefaultTexture2D(format, width, height);
		return AllocateResource(desc, localId, heapType);
	}

	AllocatedResource& GpuAllocator::AllocateResource(D3D12_RESOURCE_DESC& desc, u64 localId, u16 heapType) noexcept
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
		
		u64 resourceSize = desc.Width * desc.Height;
		auto& pool = globals::gGpuMemManager.RequestPool(resourceSize, heapType, resourceDimension, RESOURCE_TYPE_PLACED);

		AllocatedResource* resource = globals::gFlushedResourcesTable.AddResource(localId, resourceDimension);
		resource->Heap = pool.Placed.Pool;
		resource->Offset = pool.Offset;
		globals::gDevice->CreatePlacedResource(pool.Placed.Pool, pool.Offset, &desc, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&resource->Resource));

		pool.Offset += math::AlignUp(resourceSize, desc.Alignment);
		pool.AvailableSpace = pool.MaxSpaceSize - pool.Offset;

		return *resource;
	}

	AllocatedResource& GpuAllocator::AllocateUploadHeap(u64 size) noexcept
	{
		auto& pool = globals::gGpuMemManager.RequestPool(size, HEAP_TYPE_UPLOAD, RESOURCE_DIMENSION_BUFFER, RESOURCE_TYPE_COMMITTED);
		
		AllocatedResource resource;
		resource.Heap = nullptr;
		resource.Resource = pool.Committed.Pool;
		resource.Offset = pool.Offset;

		globals::gFlushedResourcesTable.SetBuffer(resource);
		auto ret = globals::gFlushedResourcesTable.GetBufferById(globals::gFlushedResourcesTable.GetBuffersCount() - 1);

		pool.Offset += math::AlignUp(size, KB(64));
		pool.AvailableSpace = pool.MaxSpaceSize - pool.Offset;

		return *ret;
	}
}