#pragma once 

#include <core/core.hpp>
#include <core/lib/array.hpp>
#include <core/debug/assert.hpp>
#include <d3d12.h>

#define HEAP_TYPES_COUNT 0x3U
#define RESOURCE_TYPES_COUNT 0x2U
#define RESOURCE_DIMENSIONS_COUNT 0x3U

#define HEAP_TYPE_MASK 0xC000000000000000
#define HEAP_TYPE_DEFAULT 0x0U
#define HEAP_TYPE_UPLOAD 0x1U
#define HEAP_TYPE_READBACK 0x2U
#define HEAP_TYPE_SHIFT 0x3EU

#define RESOURCE_TYPE_MASK 0x3000000000000000
#define RESOURCE_TYPE_COMMITTED 0x0U
#define RESOURCE_TYPE_PLACED 0x1U
#define RESOURCE_TYPE_SHIFT 0x3CU

#define RESOURCE_DIMENSION 0x0C00000000000000
#define RESOURCE_DIMENSION_BUFFER 0x0U
#define RESOURCE_DIMENSION_TEXTURE 0x1U
#define RESOURCE_DIMENSION_RTV_AND_DSV 0x2U
#define RESOURCE_DIMENSION_SHIFT 0x3AU

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

		GpuMemoryPool& RequestPool(u64 requestedSize, u16 heapType, u16 dimensionType, u16 resourceType) noexcept;

	private:
		GpuMemoryPool& CreateNewPool(u64 requestedSize, u16 heapType, u16 dimensionType, u16 resourceType) noexcept;
		ID3D12Heap* CreateHeap(u64 requestedSize, u16 heapType, u16 dimensionType) noexcept;
		ID3D12Resource* CreateCommittedPool(u64 requestedSize, u16 heapType, u16 dimensionType) noexcept;
		void Destroy() noexcept;

	private:
		Array<GpuMemoryPool> _pools[HEAP_TYPES_COUNT][RESOURCE_DIMENSIONS_COUNT][RESOURCE_TYPES_COUNT];

	};
}