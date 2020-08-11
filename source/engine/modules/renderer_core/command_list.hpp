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
	eDirect,
	eBundle,
	eCompute,
	eCopy
};

class ICommandList : public IRRCBase
{
public:
	virtual ~ICommandList() {}

	//virtual void ResourceBarrier(resources::IResourceBarrier* resourceBarrier, u32 barrierCount) = 0;
	virtual void Transition(resources::ResourceTransitionStateDesc* desc, u32 descCount) = 0;
	virtual void OMSetRenderTargetView(u32, ICPUDescriptor*, ICPUDescriptor*, bool) = 0;
	virtual void SetPipelineState(IPipelineState* pso) = 0;
	virtual void ClearRenderTarget(ICPUDescriptor*, float*) = 0;
	virtual bool Reset(ICommandAllocator*, IPipelineState*) = 0;
	/**
	 *  Copies data from src to dest
	 *  @param dest the resource that will contain copied data
	 *	@param src the resource from which data will be copied
	 *	@param offset the offset, in bytes, to the source resource
	 *	@param indexSubresource the index of first subresource
	 *	@param subresourceCount the count of the subresource that will be copied
	 *  @returns the size of copied data, in bytes
	 */
	virtual u64 UpdateSubresource(resources::IResource* dest, resources::IResource* src, u64 offset, u64 indexSubresource, u64 subresourceCount) = 0;
	virtual bool Close() = 0;

};


}