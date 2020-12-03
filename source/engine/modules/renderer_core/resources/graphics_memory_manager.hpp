#pragma once 

#include <core/core.hpp>
#include <core/lib/array.hpp>
#include <core/debug/assert.hpp>
#include <d3d12.h>

#define HEAP_TYPES_COUNT 0x3U
#define RESOURCE_TYPES_COUNT 0x2U
#define HEAP_RESOURCES_TYPES_COUNT 0x3U

#define HEAP_TYPE_MASK 0xC000000000000000
#define HEAP_TYPE_DEFAULT 0x0U
#define HEAP_TYPE_UPLOAD 0x1U
#define HEAP_TYPE_READBACK 0x2U
#define HEAP_TYPE_SHIFT 0x3EU

#define RESOURCE_TYPE_MASK 0x3000000000000000
#define RESOURCE_TYPE_COMMITTED 0x0U
#define RESOURCE_TYPE_PLACED 0x1U
#define RESOURCE_TYPE_SHIFT 0x3CU

#define HEAP_RESOURCES_TYPE 0x0C00000000000000
#define HEAP_RESOURCES_TYPE_BUFFER 0x0U
#define HEAP_RESOURCES_TYPE_TEXTURE 0x1U
#define HEAP_RESOURCES_TYPE_RTV_AND_DSV 0x2U
#define HEAP_RESOURCES_TYPE_SHIFT 0x3AU

namespace ray::renderer_core_api::resources
{
	struct GpuMemoryPool
	{
		~GpuMemoryPool()
		{
			AvailableSpace = 0;
			Committed.Pool->Release();
		}

		union
		{
			struct
			{
				ID3D12Resource* Pool;
			} Committed;

			struct
			{
				ID3D12Heap* Pool;
			} Placed;
		};

		u64 AvailableSpace;

	} ;

	class GpuMemoryManager
	{
	public:
		~GpuMemoryManager()
		{
			Destroy();
		}

		GpuMemoryPool& RequestPool(u64 requestedSize, u16 heapType, u16 heapResourcesType, u16 resourceType) noexcept;

	private:
		GpuMemoryPool& CreateNewPool(u64 requestedSize, u16 heapType, u16 heapResourcesType, u16 resourceType) noexcept;
		ID3D12Heap* CreateHeap(u64 requestedSize, u16 heapType, u16 heapResourcesType) noexcept;
		ID3D12Resource* CreateCommittedPool(u64 requestedSize, u16 heapType, u16 heapResourcesType) noexcept;
		void Destroy() noexcept;

	private:
		Array<GpuMemoryPool> _pools[HEAP_TYPES_COUNT][HEAP_RESOURCES_TYPES_COUNT][RESOURCE_TYPES_COUNT];

	};
}