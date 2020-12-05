#pragma once

#include <core/core.hpp>
#include <d3d12.h>

namespace ray::renderer_core_api::resources
{
	struct AllocatedResource
	{
		ID3D12Resource* Resource;

		// Heap is nullptr if Resource is committed
		ID3D12Heap* Heap;
		u64 Offset;
	};

	class GpuAllocator
	{
	public:
		AllocatedResource& AllocateBuffer(u64 size, u16 heapType) noexcept;
		AllocatedResource& AllocateTexture2D(DXGI_FORMAT format, u64 width, u64 height, u16 heapType) noexcept;

		// allocates placed resources
		AllocatedResource& AllocateResource(D3D12_RESOURCE_DESC& desc, u16 heapType) noexcept;

		AllocatedResource& AllocateUploadHeap(u64 size) noexcept;

		AllocatedResource& AllocateRTVorDSV() noexcept {}

	private:


	};
}