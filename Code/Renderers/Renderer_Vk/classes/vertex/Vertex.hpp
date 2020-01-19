#pragma once
#include "pch.hpp"

namespace ray::renderer::vulkan::classes::vertex
{

struct Vertex
{
	glm::vec2 position;
	glm::vec3 color;
	glm::vec2 texture;

public:
	Vertex(glm::vec2 pos, glm::vec3 color, glm::vec2 texture) 
	: position(pos), color(color), texture(texture)
	{}

	static vk::VertexInputBindingDescription getBindingDescription()
	{
		return vk::VertexInputBindingDescription(0, sizeof(Vertex));
	}

	static std::array<vk::VertexInputAttributeDescription, 3> getAttributeDescription()
	{
		return std::array<vk::VertexInputAttributeDescription, 3> 
		{vk::VertexInputAttributeDescription(0, 0, vk::Format::eR32G32Sfloat, offsetof(Vertex, position)),
			vk::VertexInputAttributeDescription(1, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, color)),
		vk::VertexInputAttributeDescription(2, 0, vk::Format::eR32G32Sfloat,offsetof(Vertex, texture))};
	}
};

}