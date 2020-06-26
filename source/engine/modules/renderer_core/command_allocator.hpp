#pragma once
#include "ray_renderer_core_base.hpp"

/*
**	Vulkan:			VkCommandPool
**	Direct3D 12:	ID3D12CommandAllocator
*/

namespace ray::renderer_core_api
{
class ICommandAllocator : public IRRCBase
{
public:
	virtual ~ICommandAllocator() {};

	virtual bool Reset() = 0;
	
};


}