#pragma once
#include "pch.hpp"

namespace ray::renderer::vulkan::classes::vertex
{

struct IndexBuffer
{
	vk::UniqueBuffer _buffer;
	vk::UniqueDeviceMemory _deviceMemory;

	void Initialize(vk::UniqueDevice const&, vk::PhysicalDevice, std::vector<u32> input);
};


}
