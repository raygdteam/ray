#pragma once
#include "pch.hpp"

namespace ray::renderer::vulkan::classes::texture
{

struct DepthBuffer
{
	vk::Format format;

	vk::UniqueImage buffer;
	vk::UniqueDeviceMemory memory;
	vk::UniqueImageView imageView;

	bool load(vk::Device device, vk::PhysicalDevice physical, vk::CommandPool cmdPool, vk::Extent2D size);
	void destruct()
	{
		imageView.reset();
		memory.reset();
		buffer.reset();
	}
};

}