#pragma once

#include <set>
#include <queue>
#include <d3d12.h>
#include <core/core.hpp>
#include <core/lib/array.hpp>
#include <core/threading/critical_section.hpp>
#include "gpu_buffer.hpp"

#define MIN_PLACED_BUFFER_SIZE 64 * 1024

namespace ray::renderer_core_api
{
	enum BuddyAllocationStrategy
	{
		ePlacedResourceStrategy,
		eManualSuballocationStrategy
	};

	struct BuddyBlock
	{
		resources::GpuBuffer* Buffer;
		ID3D12Heap* BackingHeap;
		size_t BufferSize;
		size_t Offset;
		size_t UnpaddedSize;
		u64 FenceValue;

		size_t GetSize() const noexcept { return BufferSize; }
		size_t GetOffset() const noexcept { return Offset; }

		BuddyBlock()
			: Buffer(nullptr)
			, BackingHeap(nullptr)
			, BufferSize(0)
			, Offset(0)
			, UnpaddedSize(0)
			, FenceValue(0)
		{}

		BuddyBlock(size_t heapOffset, size_t totalSize, size_t unpaddedSize);

		void InitPlaced(ID3D12Heap backingHeap, size_t numElements, size_t elementSize, void* initialData = nullptr);
		void InitFromResource(resources::GpuBuffer* buffer, size_t numElements, size_t elementSize, void* initialData = nullptr);
		void Destroy();
	};


	class BuddyAllocator
	{
	public:


	private:


	};
}