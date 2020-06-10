#pragma once
#include "ray_renderer_core_base.hpp"

#include <ray/type/extensions.hpp>
#include "command_queue.hpp"
#include "resources/resources.hpp"

/*
**	Vulkan:			VkSwapchain
**	Direct3D 12:	IDXGISwapChain3
*/

namespace ray::renderer_core_api
{
struct SwapChainDesc
{
	struct
	{
		u16 _width;
		u16 _height;
	} _back_buffer;
	
	u16 _buffer_count;
	bool _fullscreen;
	void* _output_window;
	ICommandQueue* _command_queue;
};

class ISwapChain : public IRRCBase
{
public:
	virtual ~ISwapChain() = 0;

	virtual bool Initialize(SwapChainDesc&, u32&) = 0;
	virtual bool GetBuffer(u32, resources::IResource*) = 0;
	virtual u32 GetCurrentBackBufferIndex() = 0;
	virtual bool Present(u32, u32) = 0;

};
}