#pragma once

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


};
}