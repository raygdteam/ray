#pragma once
#include "pch.hpp"
#include "UniformBufferObject.hpp"

namespace ray::renderer::vulkan::classes::uniform
{

struct UniformBuffer
{
	vk::UniqueBuffer _buffer;
	vk::UniqueDeviceMemory _deviceMemory;

	void Initialize(vk::UniqueDevice const&, vk::PhysicalDevice);
};


}