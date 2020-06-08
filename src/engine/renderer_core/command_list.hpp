#pragma once
#include "ray_renderer_core_base.hpp"
#include "command_allocator.hpp"
#include "pipeline_state.hpp"

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
	virtual bool Reset(ICommandAllocator*, IPipelineState*) = 0;
	virtual void Close() = 0;

};
}