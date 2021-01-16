#include "gpu_memory_pool.hpp"
#include <renderer_core/renderer.hpp>

void GpuMemoryPool::Create(size_t maxMemoryPoolSize, size_t index) noexcept
{
	IMemoryPool::Create(maxMemoryPoolSize, index);

	auto heapProps = DescribeHeapProps(D3D12_HEAP_TYPE_DEFAULT);
	auto heapDesc = DescribeHeap(heapProps, D3D12_HEAP_FLAG_NONE, maxMemoryPoolSize);
	gDevice->CreateHeap(&heapDesc, IID_PPV_ARGS(&_heap));
}

void GpuMemoryPool::Destroy() noexcept
{
	_heap->Release();
}
