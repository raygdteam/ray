#include "descriptor_heap.hpp"
#include "renderer_globals.hpp"

namespace ray::renderer_core_api
{
    std::vector<IDescriptorHeap*> DescriptorAllocator::sDescriptorHeapPool;
    std::mutex DescriptorAllocator::sAllocationMutex;

    ICPUDescriptor* DescriptorAllocator::Allocate(size_t count)
    {
        if (_currentHeap == nullptr || _remainingFreeHandles < count)
        {
            _currentHeap = RequestNewHeap(_type);
            _currentHeap->GetCPUDescriptorHandleForHeapStart(_currentHandle);
            _remainingFreeHandles = sNumDescriptorsPerHeap;

            if (_descriptorSize == 0)
                _descriptorSize = gDevice->GetDescriptorHandleIncrementSize();
        }

        ICPUDescriptor* ret = _currentHandle;
        _currentHandle->Offset(count);
        _remainingFreeHandles -= count;


        return ret;
    }

    void DescriptorAllocator::DestroyAll()
    {
        sDescriptorHeapPool.clear();
    }

    IDescriptorHeap* DescriptorAllocator::RequestNewHeap(DescriptorHeapType type)
    {
        std::lock_guard<std::mutex> LockGuard(sAllocationMutex);

        DescriptorHeapDesc desc;
        desc.NumDescriptors = sNumDescriptorsPerHeap;
        desc.Type = type;
        desc.ShaderVisible = false;

        IDescriptorHeap* heap = nullptr;
        gDevice->CreateDescriptorHeap(desc, heap);
        sDescriptorHeapPool.push_back(heap);
        return heap;
    }

}

