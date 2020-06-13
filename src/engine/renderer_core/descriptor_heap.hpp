#pragma once
#include "ray_renderer_core_base.hpp"
#include <ray/type/extensions.hpp>

/*
**	Vulkan:			VkDescriptorPool
**	Direct3D 12:	ID3D12DescriptorHeap
*/

namespace ray::renderer_core_api
{
enum class DescriptorHeapType
{
	descriptor_heap_type_rtv,
	descriptor_heap_type_uav_srv_cbv,
	descriptor_heap_type_sampler,
	descriptor_heap_type_dsv
};

struct DescriptorHeapDesc
{
	u32 _num_descriptors;
	DescriptorHeapType _type;
	bool _shader_visible;
};

class IDescriptorHeap : public IRRCBase
{
public:
	virtual ~IDescriptorHeap() {}

};


class IDescriptor : public IRRCBase
{
public:
	IDescriptor() : _size(0) {}
	virtual bool Initialize(IDescriptorHeap*) = 0;
	virtual bool Offset(u32) = 0;

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

}