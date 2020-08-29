#pragma once
#include "ray_renderer_core_base.hpp"
#include <core/core.hpp>
#include <vector>
#include <core/threading/critical_section.hpp>
#include <d3d12.h>

#define D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN static_cast<D3D12_GPU_VIRTUAL_ADDRESS>(-1)

/*
**	Vulkan:			VkDescriptorPool
**	Direct3D 12:	ID3D12DescriptorHeap
*/

namespace ray::renderer_core_api
{
	class DescriptorAllocator
	{
	public:
		DescriptorAllocator(D3D12_DESCRIPTOR_HEAP_TYPE type) : _currentHeap(nullptr), _type(type) {}

		D3D12_CPU_DESCRIPTOR_HANDLE Allocate(size_t count = 1);
		static void DestroyAll();

	private:
		static const uint32_t sNumDescriptorsPerHeap = 256;
		static ray::CriticalSection sAllocationMutex;
		static std::vector<ID3D12DescriptorHeap*> sDescriptorHeapPool;
		static ID3D12DescriptorHeap* RequestNewHeap(D3D12_DESCRIPTOR_HEAP_TYPE Type);

		ID3D12DescriptorHeap* _currentHeap;
		D3D12_DESCRIPTOR_HEAP_TYPE _type;
		std::vector<ID3D12DescriptorHeap*> _descriptorHeapPool;
		size_t _remainingFreeHandles;
		size_t _descriptorSize;
		D3D12_CPU_DESCRIPTOR_HANDLE _currentHandle;

	};

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

	class UserDescriptorHeap
	{
	public:
		UserDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type, size_t numDescriptors)
		{
			_heapDesc = D3D12_DESCRIPTOR_HEAP_DESC{
				.Type = type,
				.NumDescriptors = numDescriptors,
				.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
				.NodeMask = 1
			};
		}

		void Create();
		
		DescriptorHandle Allocate(u32 coutn = 1);

		bool HasAvailableSpace(u32 count) const noexcept { return count <= _numFreeDescriptors; }
		DescriptorHandle GetDescriptorAtOffset(u32 offset) const noexcept { return _firstHandle + offset * _descriptorSize; }
		bool ValidateHandle(const DescriptorHandle& handle) const noexcept;

		ID3D12DescriptorHeap* GetHeapPointer() const noexcept { return _heap; }

	private:
		ID3D12DescriptorHeap* _heap;
		D3D12_DESCRIPTOR_HEAP_DESC _heapDesc;
		u32 _descriptorSize;
		u32 _numFreeDescriptors;
		DescriptorHandle _firstHandle;
		DescriptorHandle _nextFreeHandle;

	};

}