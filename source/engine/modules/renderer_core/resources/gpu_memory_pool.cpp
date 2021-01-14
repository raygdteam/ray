#include "gpu_memory_pool.hpp"
#include <renderer_core/renderer.hpp>

namespace ray::renderer_core_api::resources
{
    void GpuMemoryPool::Create(size_t maxMemoryPoolSize, size_t index) noexcept
    {
        IMemoryPool::Create(maxMemoryPoolSize, index);

        auto heapProps = ray::dx12::DescribeHeapProps(D3D12_HEAP_TYPE_DEFAULT);
        auto heapDesc = ray::dx12::DescribeHeap(heapProps, D3D12_HEAP_FLAG_NONE, maxMemoryPoolSize);
        globals::gDevice->CreateHeap(&heapDesc, IID_PPV_ARGS(&_heap));
    }

    void GpuMemoryPool::Destroy() noexcept
    {
        _heap->Release();
    }
}