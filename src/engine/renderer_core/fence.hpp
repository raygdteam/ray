#pragma once
#include "ray_renderer_core_base.hpp"
#include <ray/type/extensions.hpp>

/*
**	Vulkan:			VkFence
**	Direct3D 12:	ID3D12Fence
*/

namespace ray::renderer_core_api
{

class IFenceEvent : public IRRCBase
{
public:
	virtual ~IFenceEvent() {}

	virtual u32 WaitFor() = 0;
	virtual u32 WaitFor(u32) = 0;
};


class IFence : public IRRCBase
{
public:
	virtual ~IFence() {}

	virtual bool SetEventOnCompletion(IFenceEvent*, u64) = 0;
	virtual u64 GetCompletedValue() = 0;
};


}