#include "gpu_memory_pool.hpp"
#include <renderer_core/renderer.hpp>
#include <core/log/log.hpp>

GpuMemoryPool::GpuMemoryPool(size_t poolSize, size_t poolIndex, D3D12_HEAP_FLAGS flags) noexcept
	: IMemoryPool(poolSize, poolIndex)
	, _allocator(poolSize)
{
	auto heapProps = DescribeHeapProps(D3D12_HEAP_TYPE_DEFAULT);
	auto heapDesc = DescribeHeap(heapProps, flags, poolSize);
	MAYBE_UNUSED auto hr = gDevice->CreateHeap(&heapDesc, IID_PPV_ARGS(&_heap));
	ray_assert(hr == S_OK, "Cannot create a gpu pool");

	IRenderer::sRendererStats.AllocatedGpuMemory += poolSize;
	gRendererLogger->Log("Allocating GpuMemoryPool. Allocated Gpu Memory: %u KB", IRenderer::sRendererStats.AllocatedGpuMemory / 1024);
}

bool GpuMemoryPool::IsEnough(size_t size) const noexcept
{
	return size <= _allocator.GetMaxAvailableSpace();
}

size_t GpuMemoryPool::Allocate(size_t size) noexcept
{
	return  _allocator.Allocate(size);
}

void GpuMemoryPool::Free(size_t offset, size_t size) noexcept
{
	_allocator.Free(offset, size);
}

void GpuMemoryPool::Destroy() noexcept
{
	_heap->Release();
	IRenderer::sRendererStats.AllocatedGpuMemory -= this->_poolSize;
	gRendererLogger->Log("Releasing GpuMemoryPool. Allocated Gpu Memory: %u KB", IRenderer::sRendererStats.AllocatedGpuMemory / 1024);
}

GpuTextureMemoryPool::GpuTextureMemoryPool(size_t poolSize, size_t poolIndex)
	: GpuMemoryPool(poolSize, poolIndex, D3D12_HEAP_FLAG_ALLOW_ONLY_NON_RT_DS_TEXTURES)
{}

GpuBufferMemoryPool::GpuBufferMemoryPool(size_t poolSize, size_t poolIndex)
	: GpuMemoryPool(poolSize, poolIndex, D3D12_HEAP_FLAG_ALLOW_ONLY_BUFFERS)
{}

GpuPixelBufferMemoryPool::GpuPixelBufferMemoryPool(size_t poolSize, size_t poolIndex)
	: GpuMemoryPool(poolSize, poolIndex, D3D12_HEAP_FLAG_ALLOW_ONLY_RT_DS_TEXTURES)
{}
