#pragma once
#include "ray_renderer_core_base.hpp"
#include <core/core.hpp>
#include <core/lib/array.hpp>
#include <core/threading/critical_section.hpp>
#include <d3d12.h>

#define D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN static_cast<D3D12_GPU_VIRTUAL_ADDRESS>(-1)

class DescriptorHandle
{
public:
	DescriptorHandle()
	{
		_CpuHandle.ptr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
		_GpuHandle.ptr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
	}

	DescriptorHandle(D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle)
		: _CpuHandle(cpuHandle)
	{
		_GpuHandle.ptr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
	}

	DescriptorHandle(D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle)
		: _CpuHandle(cpuHandle)
		, _GpuHandle(gpuHandle)
	{}

	DescriptorHandle operator + (size_t offsetScaledByDescriptorSize) const noexcept
	{
		auto ret = *this;
		ret += offsetScaledByDescriptorSize;
		return ret;
	}

	void operator += (size_t offsetScaledByDescriptorSize)
	{
		if (_CpuHandle.ptr != D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN)
			_CpuHandle.ptr += offsetScaledByDescriptorSize;
		if (_GpuHandle.ptr != D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN)
			_GpuHandle.ptr += offsetScaledByDescriptorSize;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle() const noexcept
	{
		return _CpuHandle;
	}

	D3D12_GPU_DESCRIPTOR_HANDLE GetGpuHandle() const noexcept
	{
		return _GpuHandle;
	}

	bool IsNull() const noexcept
	{
		return _CpuHandle.ptr == D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
	}

	bool IsShaderVisible() const noexcept
	{
		return _GpuHandle.ptr != D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
	}

private:
	D3D12_CPU_DESCRIPTOR_HANDLE _CpuHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE _GpuHandle;

};

class DescriptorHeap
{
public:
	DescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type, D3D12_DESCRIPTOR_HEAP_FLAGS flag, size_t numDescriptors)
	{
		_heapDesc = D3D12_DESCRIPTOR_HEAP_DESC{
			.Type = type,
			.NumDescriptors = u32(numDescriptors), // TODO: workaround
			.Flags = flag,
			.NodeMask = 1
		};
	}

	void Create();

	DescriptorHandle Allocate(u32 count = 1);

	bool HasAvailableSpace(u32 count) const noexcept { return count <= _numFreeDescriptors; }
	DescriptorHandle GetDescriptorAtOffset(u32 offset) const noexcept { return _firstHandle + offset * _descriptorSize; }
	bool ValidateHandle(const DescriptorHandle& handle) const noexcept;

	ID3D12DescriptorHeap* GetHeapPointer() const noexcept { return _heap; }
	u32 GetMaxDescriptorsCount() const noexcept
	{
		return _heapDesc.NumDescriptors;
	}

private:
	ID3D12DescriptorHeap* _heap;
	D3D12_DESCRIPTOR_HEAP_DESC _heapDesc;
	u32 _descriptorSize;
	u32 _numFreeDescriptors;
	DescriptorHandle _firstHandle;
	DescriptorHandle _nextFreeHandle;

};

class DescriptorHeapsManager
{
private:
	Array<DescriptorHeap> _heaps[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES];
	u32 _currentHeaps[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES];

public:
	DescriptorHeap& CreateHeap(D3D12_DESCRIPTOR_HEAP_TYPE type, D3D12_DESCRIPTOR_HEAP_FLAGS flag, u32 elementsCount) noexcept;

public:
	DescriptorHeap& GetCurrentHeap(D3D12_DESCRIPTOR_HEAP_TYPE type, bool bAllowCreateNewHeap) noexcept;

	DescriptorHeap& GetCurrentRTV_Heap(bool bAllowCreateNewHeap) noexcept
	{
		return GetCurrentHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, bAllowCreateNewHeap);
	}

	DescriptorHeap& GetCurrentCBV_SRV_UAV_Heap(bool bAllowCreateNewHeap) noexcept
	{
		return GetCurrentHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, bAllowCreateNewHeap);
	}

	DescriptorHeap& GetCurrentSamplerHeap(bool bAllowCreateNewHeap) noexcept
	{
		return GetCurrentHeap(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, bAllowCreateNewHeap);
	}

	DescriptorHeap& GetCurrentDSV_Heap(bool bAllowCreateNewHeap) noexcept
	{
		return GetCurrentHeap(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, bAllowCreateNewHeap);
	}

};
