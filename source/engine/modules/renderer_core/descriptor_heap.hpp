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
enum class DescriptorHeapType
{
	eRTV,
	eUAV_SRV_CBV,
	eSampler,
	eDSV
};

struct DescriptorHeapDesc
{
	u32 NumDescriptors;
	DescriptorHeapType Type;
	bool ShaderVisible;
};

class ICPUDescriptor;
class IGPUDescriptor;

class IDescriptorHeap : public IRRCBase
{
public:
	virtual ~IDescriptorHeap() {}
	
	virtual void GetCPUDescriptorHandleForHeapStart(ICPUDescriptor* outHandle) noexcept = 0;
	virtual void GetGPUDescriptorHandleForHeapStart(IGPUDescriptor* outHandle) noexcept = 0;
};


class IDescriptor : public IRRCBase
{
public:
	IDescriptor() : _size(0) {}
	virtual bool Offset(u32 step) = 0;
	virtual bool Increment() = 0;

	size_t GetDescriptorSize() { return _size; }
	void SetDescriptorSize(size_t size) { _size = size; }

private:
	size_t _size;

};

class ICPUDescriptor : public IDescriptor
{
public:
	virtual ~ICPUDescriptor() {}
};

class IGPUDescriptor : public IDescriptor
{

};

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