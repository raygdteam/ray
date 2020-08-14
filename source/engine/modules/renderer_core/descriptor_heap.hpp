#pragma once
#include "ray_renderer_core_base.hpp"
#include <core/core.hpp>
#include <vector>
#include <core/threading/critical_section.hpp>
#include <d3d12.h>

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
	void DestroyAll();

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

}