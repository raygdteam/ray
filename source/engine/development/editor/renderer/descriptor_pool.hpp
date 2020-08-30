#pragma once
#include <core/core.hpp>
#include <editor/renderer/loader/volk.h>

class DescriptorPool
{
public:
	// temp
	static VkDescriptorPool CreateDescriptorPool(VkDevice device);
};
