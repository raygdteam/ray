#include "pch.hpp"
#include "Texture.hpp"
#include <stb_image.h>
#include "utils/utils.hpp"

namespace ray::renderer::vulkan::classes::texture
{

bool Texture::load(pcstr path, vk::Device device, vk::PhysicalDevice physical)
{
	int texWidth, texHeight, texChannels;
	pcstr buf = ray::file_system::read_file(path);
	stbi_uc* pixels = stbi_load_from_memory((stbi_uc*)buf, ray::file_system::file_size(path), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

	VkDeviceSize imageSize = texWidth * texHeight * 4;

	if(!pixels)
	{
		spdlog::error("vulkanrenderer: failed to load image '{}'", path);
		return false;
	}

	auto buffer = device.createBufferUnique(vk::BufferCreateInfo(vk::BufferCreateFlags(), imageSize, vk::BufferUsageFlagBits::eUniformBuffer, vk::SharingMode::eExclusive));
	auto memoryRequirements = device.getBufferMemoryRequirements(buffer.get());

	auto deviceMemory = device.allocateMemoryUnique(vk::MemoryAllocateInfo(
		memoryRequirements.size, utilities::findMemoryType(memoryRequirements.memoryTypeBits,
			vk::MemoryPropertyFlagBits::eHostVisible | vk::
			MemoryPropertyFlagBits::eHostCoherent, physical)));

	device.bindBufferMemory(buffer.get(), deviceMemory.get(), 0);

	void* data = nullptr;
	device.mapMemory(deviceMemory.get(), 0, imageSize, vk::MemoryMapFlags(), &data);
		memcpy(data, pixels, imageSize);
	device.unmapMemory(deviceMemory.get());

	vk::ImageCreateInfo imageInfo = {};
	imageInfo.imageType = vk::ImageType::e2D;
	imageInfo.extent.width = static_cast<uint32_t>(texWidth);
	imageInfo.extent.height = static_cast<uint32_t>(texHeight);
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = vk::Format::eR8G8B8A8Unorm;
	imageInfo.tiling = vk::ImageTiling::eOptimal;
	imageInfo.initialLayout = vk::ImageLayout::eUndefined;
	imageInfo.usage = vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled;
	imageInfo.sharingMode = vk::SharingMode::eExclusive;
	imageInfo.samples = vk::SampleCountFlagBits::e1;

	texture = device.createImageUnique(imageInfo);

	return true;
}

}
