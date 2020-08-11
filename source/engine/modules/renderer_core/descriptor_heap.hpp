#pragma once
#include "ray_renderer_core_base.hpp"
#include <core/core.hpp>

#include <mutex>
#include <vector>

/*
**	Vulkan:			VkDescriptorPool
**	Direct3D 12:	ID3D12DescriptorHeap
*/

namespace ray::renderer_core_api
{
class DescriptorAllocator
{
public:
	DescriptorAllocator(DescriptorHeapType type) : _type(type), _currentHeap(nullptr) {}

	ICPUDescriptor* Allocate(size_t count = 1);
	void DestroyAll();

private:
	static const uint32_t sNumDescriptorsPerHeap = 256;
	static std::mutex sAllocationMutex;
	static std::vector<IDescriptorHeap*> sDescriptorHeapPool;
	static IDescriptorHeap* RequestNewHeap(DescriptorHeapType Type);

	IDescriptorHeap* _currentHeap;
	DescriptorHeapType _type;
	std::vector<IDescriptorHeap*> _descriptorHeapPool;
	size_t _remainingFreeHandles;
	size_t _descriptorSize;
	ICPUDescriptor* _currentHandle;

};

}