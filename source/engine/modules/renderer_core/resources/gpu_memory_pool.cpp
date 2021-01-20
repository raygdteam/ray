#include "gpu_memory_pool.hpp"
#include <renderer_core/renderer.hpp>

void GpuMemoryPool::Create(size_t maxMemoryPoolSize, size_t index) noexcept
{
	IMemoryPool::Create(maxMemoryPoolSize, index);

	auto heapProps = DescribeHeapProps(D3D12_HEAP_TYPE_DEFAULT);
	auto heapDesc = DescribeHeap(heapProps, D3D12_HEAP_FLAG_NONE, maxMemoryPoolSize);
	MAYBE_UNUSED auto hr = gDevice->CreateHeap(&heapDesc, IID_PPV_ARGS(&_heap));
	ray_assert(hr == S_OK, "Cannot create a gpu pool")
}

void GpuMemoryPool::Destroy() noexcept
{
	_heap->Release();
}

void GpuTextureMemoryPool::Create(size_t maxMemoryPoolSize, size_t index) noexcept
{
	IMemoryPool::Create(maxMemoryPoolSize, index);

	auto heapProps = DescribeHeapProps(D3D12_HEAP_TYPE_DEFAULT);
	auto heapDesc = DescribeHeap(heapProps, D3D12_HEAP_FLAG_ALLOW_ONLY_NON_RT_DS_TEXTURES, maxMemoryPoolSize);
	MAYBE_UNUSED auto hr = gDevice->CreateHeap(&heapDesc, IID_PPV_ARGS(&_heap));
	ray_assert(hr == S_OK, "Cannot create a gpu pool")
}

void GpuTextureMemoryPool::Destroy() noexcept
{
	_heap->Release();
}

void GpuBufferMemoryPool::Create(size_t maxMemoryPoolSize, size_t index) noexcept
{
	IMemoryPool::Create(maxMemoryPoolSize, index);

	auto heapProps = DescribeHeapProps(D3D12_HEAP_TYPE_DEFAULT);
	auto heapDesc = DescribeHeap(heapProps, D3D12_HEAP_FLAG_ALLOW_ONLY_BUFFERS, maxMemoryPoolSize);
	MAYBE_UNUSED auto hr = gDevice->CreateHeap(&heapDesc, IID_PPV_ARGS(&_heap));
	ray_assert(hr == S_OK, "Cannot create a gpu pool")
}

void GpuBufferMemoryPool::Destroy() noexcept
{
	_heap->Release();
}

void GpuPixelBufferMemoryPool::Create(size_t maxMemoryPoolSize, size_t index) noexcept
{
	IMemoryPool::Create(maxMemoryPoolSize, index);

	auto heapProps = DescribeHeapProps(D3D12_HEAP_TYPE_DEFAULT);
	auto heapDesc = DescribeHeap(heapProps, D3D12_HEAP_FLAG_ALLOW_ONLY_RT_DS_TEXTURES, maxMemoryPoolSize);
	MAYBE_UNUSED auto hr = gDevice->CreateHeap(&heapDesc, IID_PPV_ARGS(&_heap));
	ray_assert(hr == S_OK, "Cannot create a gpu pool")
}

void GpuPixelBufferMemoryPool::Destroy() noexcept
{
	_heap->Release();
}
