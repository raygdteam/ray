#pragma once
#include "pch.hpp"
#include "Vertex.hpp"

namespace ray::renderer::vulkan::classes::vertex
{

struct VertexBuffer
{
	vk::UniqueBuffer _buffer;
	vk::UniqueDeviceMemory _deviceMemory;

	void Initialize(vk::UniqueDevice const&, vk::PhysicalDevice, std::vector<vertex::Vertex> input);
};

}