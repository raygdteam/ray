#include "pch.hpp"
#include "UniformBuffer.hpp"
#include "utils/utils.hpp"

namespace ray::renderer::vulkan::classes::uniform
{

void UniformBuffer::Initialize(vk::UniqueDevice const& device, vk::PhysicalDevice physical)
{
	auto size = sizeof(UniformBufferObject);
	_buffer = device->createBufferUnique(vk::BufferCreateInfo(vk::BufferCreateFlags(), size, vk::BufferUsageFlagBits::eUniformBuffer, vk::SharingMode::eExclusive));
	auto memoryRequirements = device->getBufferMemoryRequirements(_buffer.get());

	_deviceMemory = device->allocateMemoryUnique(vk::MemoryAllocateInfo(
		memoryRequirements.size, utilities::findMemoryType(memoryRequirements.memoryTypeBits,
			vk::MemoryPropertyFlagBits::eHostVisible | vk::
			MemoryPropertyFlagBits::eHostCoherent, physical)));

	device->bindBufferMemory(_buffer.get(), _deviceMemory.get(), 0);
}

}