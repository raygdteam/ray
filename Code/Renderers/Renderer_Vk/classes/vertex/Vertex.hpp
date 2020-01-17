#pragma once
#include "pch.hpp"

namespace ray::renderer::vulkan::classes::vertex
{

struct Vertex
{
	glm::vec2 position;
	glm::vec3 color;

public:
	Vertex(glm::vec2 pos, glm::vec3 color) 
	: position(pos), color(color)
	{}

	static vk::VertexInputBindingDescription getBindingDescription()
	{
		return vk::VertexInputBindingDescription(0, sizeof(Vertex));
	}

	static std::array<vk::VertexInputAttributeDescription, 2> getAttributeDescription()
	{
		return std::array<vk::VertexInputAttributeDescription, 2> 
		{vk::VertexInputAttributeDescription(0, 0, vk::Format::eR32G32Sfloat, offsetof(Vertex, position)),
			vk::VertexInputAttributeDescription(1, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, color))};
	}
};

}