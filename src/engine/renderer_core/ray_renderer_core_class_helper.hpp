#pragma once

#include "device.hpp"
#include "swap_chain.hpp"
#include "resources/resources.hpp"

namespace ray::renderer_core_api
{
	class IRRCClassHelper
	{
	public:
		virtual ICommandAllocator* CreateCommandAllocator() = 0;
		virtual ICommandList* CreateCommandList() = 0;
		virtual ICommandQueue* CreateCommandQueue() = 0;
		virtual IDescriptorHeap* CreateDescriptorHeap() = 0;
		virtual IFence* CreateFence() = 0;
		virtual IDevice* CreateDevice() = 0;
		virtual ISwapChain* CreateSwapChain() = 0;
		virtual resources::IResource* CreateResource() = 0;

	};
}