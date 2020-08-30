#pragma once
#include <editor/renderer/loader/volk.h>

class RenderPass
{
public:
	// temp
	static VkRenderPass CreateRenderPass(VkDevice device, VkFormat swapchainFormat);
};
