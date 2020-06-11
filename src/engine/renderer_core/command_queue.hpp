#pragma once
#include "command_list.hpp"
#include "fence.hpp"

/*
**	Vulkan:			VkQueue
**	Direct3D 12:	ID3D12CommandQueue
*/

namespace ray::renderer_core_api
{

struct CommandQueueDesc
{
	//TODO:
};

class ICommandQueue : public IRRCBase
{
public:
	virtual ~ICommandQueue() {}

	virtual void SetCommandLists(ICommandList**, size_t) = 0;
	virtual void ExecuteCommandLists() = 0;
	virtual bool Signal(IFence*, u32) = 0;

};


}