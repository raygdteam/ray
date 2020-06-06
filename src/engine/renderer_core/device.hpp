#pragma once

#include "ray_renderer_core_base.hpp"
#include "command_queue.hpp"
#include "command_allocator.hpp"
#include "command_list.hpp"
#include "descriptor_heap.hpp"
#include "fence.hpp"
#include "pipeline_state.hpp"

/*
**	Vulkan:			VkDevice
**	Direct3D 12:	ID3D12Device
*/

namespace ray::renderer_core_api
{
	//temp
	struct RenderTargetViewDesc {};

class IDevice : public IRRCBase
{
public:
	virtual bool Initialize() = 0;
	virtual bool CreateCommandQueue(CommandQueueDesc&, ICommandQueue*) = 0;
	virtual bool CreateDescriptorHeap(DescriptorHeapDesc&, IDescriptorHeap*) = 0;
	virtual void CreateRenderTargetView(resources::IResource*, RenderTargetViewDesc&, ICPUDescriptor*) = 0;
	virtual bool CreateCommandAllocator(ICommandAllocator*, CommandListType) = 0;
	virtual bool CreateCommandList(ICommandList*, ICommandAllocator*, IPipelineState*, CommandListType) = 0;
	virtual s32 GetDescriptorHandleIncrementSize(DescriptorHeapType) = 0;

};
}

