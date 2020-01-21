#include "pch.hpp"
#include "VertexBuffer.hpp"
#include "utils/utils.hpp"

namespace ray::renderer::vulkan::classes::vertex
{

void VertexBuffer::Initialize(vk::UniqueDevice const& device, vk::PhysicalDevice physical, std::vector<vertex::Vertex> input)
{
	auto size = sizeof(input[0]) * input.size();
	_buffer = device->createBufferUnique(vk::BufferCreateInfo(vk::BufferCreateFlags(), size, vk::BufferUsageFlagBits::eVertexBuffer, vk::SharingMode::eExclusive));
	auto memoryRequirements = device->getBufferMemoryRequirements(_buffer.get());

	_deviceMemory = device->allocateMemoryUnique(vk::MemoryAllocateInfo(
		memoryRequirements.size, utilities::findMemoryType(memoryRequirements.memoryTypeBits,
															vk::MemoryPropertyFlagBits::eHostVisible | vk::
															MemoryPropertyFlagBits::eHostCoherent, physical)));

	device->bindBufferMemory(_buffer.get(), _deviceMemory.get(), 0);

	void* data;
	device->mapMemory(_deviceMemory.get(), 0, size, vk::MemoryMapFlags(), &data);
		memcpy(data, input.data(), size);
	device->unmapMemory(_deviceMemory.get());
}

}
