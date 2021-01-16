#include "descriptor_heap.hpp"
#include <core/debug/assert.hpp>
#include "renderer.hpp"

// ------------------------------- DESCRIPTOR HEAP ------------------------------- //

void DescriptorHeap::Create()
{
	auto hr = gDevice->CreateDescriptorHeap(&_heapDesc, IID_PPV_ARGS(&_heap));
	check(hr == S_OK)

		D3D12_GPU_DESCRIPTOR_HANDLE nullHandle
	{
		.ptr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN
	};
	auto gpuHandle = _heapDesc.Flags & D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE ? _heap->GetGPUDescriptorHandleForHeapStart() : nullHandle;

	_firstHandle = DescriptorHandle(_heap->GetCPUDescriptorHandleForHeapStart(), gpuHandle);
	_descriptorSize = gDevice->GetDescriptorHandleIncrementSize(_heapDesc.Type);
	_numFreeDescriptors = _heapDesc.NumDescriptors;
	_nextFreeHandle = _firstHandle;
}

DescriptorHandle DescriptorHeap::Allocate(u32 count)
{
	ray_assert(HasAvailableSpace(count), "Descriptor Heap out of space")
		auto ret = _nextFreeHandle;
	_nextFreeHandle += count * _descriptorSize;
	return ret;
}

bool DescriptorHeap::ValidateHandle(const DescriptorHandle& handle) const noexcept
{
	if (handle.GetCpuHandle().ptr < _firstHandle.GetCpuHandle().ptr ||
		handle.GetCpuHandle().ptr >= _firstHandle.GetCpuHandle().ptr + _heapDesc.NumDescriptors * _descriptorSize)
		return false;

	if (_heapDesc.Flags & D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE &&
		handle.GetGpuHandle().ptr - _firstHandle.GetGpuHandle().ptr !=
		handle.GetCpuHandle().ptr - _firstHandle.GetCpuHandle().ptr)
		return false;

	return true;
}

// ------------------------------- DESCRIPTOR HEAPS MANAGER ------------------------------- //

DescriptorHeap& DescriptorHeapsManager::CreateHeap(D3D12_DESCRIPTOR_HEAP_TYPE type, D3D12_DESCRIPTOR_HEAP_FLAGS flag, u32 elementsCount) noexcept
{
	_heaps[type].EmplaceBack(type, flag, elementsCount);
	_currentHeaps[type] = _heaps[type].Size() - 1;

	return _heaps[type].At(_currentHeaps[type]);
}

DescriptorHeap& DescriptorHeapsManager::GetCurrentHeap(D3D12_DESCRIPTOR_HEAP_TYPE type, bool bAllowCreateNewHeap) noexcept
{
	// TODO:
	// too many kostils
	auto& heaps = _heaps[type];
	u32 currentHeap = _currentHeaps[type];

	if (heaps.empty())
		return CreateHeap(type, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, heaps.At(currentHeap).GetMaxDescriptorsCount());

	if (!bAllowCreateNewHeap || heaps.At(currentHeap).HasAvailableSpace(1))
		return heaps.At(currentHeap);

	return CreateHeap(type, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, heaps.At(currentHeap).GetMaxDescriptorsCount());
}
