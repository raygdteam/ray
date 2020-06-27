#pragma once
#include "ray_renderer_core_base.hpp"
#include "command_allocator.hpp"
#include "descriptor_heap.hpp"
#include "pipeline_state.hpp"
#include "resources/resources.hpp"
#include <core/core.hpp>

/*
**	Vulkan:			VkCommandBuffer
**	Direct3D 12:	ID3D12CommandList / ID3D12GraphicsCommandList
*/

namespace ray::renderer_core_api
{
enum class CommandListType
{
	direct,
	bundle,
	compute,
	copy
};

class ICommandList : public IRRCBase
{
public:
	virtual ~ICommandList() {}

	virtual void ResourceBarrier(resources::IResourceBarrier*, u32) = 0;
	virtual void OMSetRenderTargetView(u32, ICPUDescriptor*, ICPUDescriptor*, bool) = 0;
	virtual void ClearRenderTarget(ICPUDescriptor*, float*) = 0;
	virtual bool Reset(ICommandAllocator*, IPipelineState*) = 0;
	virtual bool Close() = 0;

};


}