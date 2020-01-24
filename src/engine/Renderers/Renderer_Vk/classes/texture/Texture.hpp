#pragma once
#include "pch.hpp"

namespace ray::renderer::vulkan::classes::texture
{

struct Texture
{
	vk::UniqueImage texture;
	vk::UniqueDeviceMemory memory;
	vk::UniqueImageView textureView;
	vk::UniqueSampler sampler;

	bool load(pcstr path, vk::Device device, vk::PhysicalDevice physical, vk::CommandPool cmdPool);
};

}