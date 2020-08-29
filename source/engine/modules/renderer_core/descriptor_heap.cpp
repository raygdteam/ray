#include "descriptor_heap.hpp"
#include <core/debug/assert.hpp>
#include "renderer.hpp"

namespace ray::renderer_core_api
{
    // ------------------------------- DESCRIPTOR ALLOCATOR ------------------------------- //

    std::vector<ID3D12DescriptorHeap*> DescriptorAllocator::sDescriptorHeapPool;
    ray::CriticalSection DescriptorAllocator::sAllocationMutex;

    D3D12_CPU_DESCRIPTOR_HANDLE DescriptorAllocator::Allocate(size_t count)
    {
        if (_currentHeap == nullptr || _remainingFreeHandles < count)
        {
            _currentHeap = RequestNewHeap(_type);
            _currentHandle = _currentHeap->GetCPUDescriptorHandleForHeapStart();
            _remainingFreeHandles = sNumDescriptorsPerHeap;

            if (_descriptorSize == 0)
                _descriptorSize = globals::gDevice->GetDescriptorHandleIncrementSize(_type);
        }

        D3D12_CPU_DESCRIPTOR_HANDLE ret = _currentHandle;
        _currentHandle.ptr += count * _descriptorSize;
        _remainingFreeHandles -= count;


        return ret;
    }

    void DescriptorAllocator::DestroyAll()
    {
        sDescriptorHeapPool.clear();
    }

    ID3D12DescriptorHeap* DescriptorAllocator::RequestNewHeap(D3D12_DESCRIPTOR_HEAP_TYPE type)
    {
        sAllocationMutex.Enter();

        D3D12_DESCRIPTOR_HEAP_DESC desc;
        desc.NumDescriptors = sNumDescriptorsPerHeap;
        desc.Type = type;
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        desc.NodeMask = 1;

        ID3D12DescriptorHeap* heap = nullptr;
        globals::gDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap));
        sDescriptorHeapPool.push_back(heap);

        sAllocationMutex.Leave();

        return heap;
    }

    // ------------------------------- USER DESCRIPTOR HEAP ------------------------------- //

    void UserDescriptorHeap::Create()
    {
        auto hr = globals::gDevice->CreateDescriptorHeap(&_heapDesc, IID_PPV_ARGS(&_heap));
        check(hr == S_OK)

        _firstHandle = DescriptorHandle(_heap->GetCPUDescriptorHandleForHeapStart(), _heap->GetGPUDescriptorHandleForHeapStart());
        _descriptorSize = globals::gDevice->GetDescriptorHandleIncrementSize(_heapDesc.Type);
        _numFreeDescriptors = _heapDesc.NumDescriptors;
        _nextFreeHandle = _firstHandle;
    }

    DescriptorHandle UserDescriptorHeap::Allocate(u32 count)
    {
        ray_assert(HasAvailableSpace(count), "Descriptor Heap out of space")
        auto ret = _nextFreeHandle;
        _nextFreeHandle += count * _descriptorSize;
        return ret;
    }

    bool UserDescriptorHeap::ValidateHandle(const DescriptorHandle& handle) const noexcept
    {
        if (handle.GetCpuHandle().ptr < _firstHandle.GetCpuHandle().ptr ||
            handle.GetCpuHandle().ptr >= _firstHandle.GetCpuHandle().ptr + _heapDesc.NumDescriptors * _descriptorSize)
            return false;

        if (handle.GetGpuHandle().ptr - _firstHandle.GetGpuHandle().ptr !=
            handle.GetCpuHandle().ptr - _firstHandle.GetCpuHandle().ptr)
            return false;

        return true;
    }

}

