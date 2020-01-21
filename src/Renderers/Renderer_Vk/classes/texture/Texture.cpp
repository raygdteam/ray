#include "pch.hpp"
#include "Texture.hpp"
#include <stb_image.h>
#include "utils/utils.hpp"

namespace ray::renderer::vulkan::classes::texture
{

bool Texture::load(pcstr path, vk::Device device, vk::PhysicalDevice physical, vk::CommandPool cmdPool)
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

	auto buffer = device.createBufferUnique(vk::BufferCreateInfo(vk::BufferCreateFlags(), imageSize, vk::BufferUsageFlagBits::eTransferSrc, vk::SharingMode::eExclusive));
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

	memoryRequirements = device.getImageMemoryRequirements(texture.get());
	
	memory = device.allocateMemoryUnique(vk::MemoryAllocateInfo(memoryRequirements.size, utilities::findMemoryType(memoryRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal, physical)));
	device.bindImageMemory(texture.get(), memory.get(), 0);

	/** transition  undefined ==> transferdstoptimal **/
	{
		auto cmd = utilities::begin_single_time_cmd_buffer(cmdPool, device);

		vk::ImageMemoryBarrier barrier;

		// undefined => transfer dst optimal
		barrier = vk::ImageMemoryBarrier(vk::AccessFlags(), vk::AccessFlagBits::eTransferWrite, vk::ImageLayout::eUndefined,
			vk::ImageLayout::eTransferDstOptimal, VK_QUEUE_FAMILY_IGNORED, VK_QUEUE_FAMILY_IGNORED,
			texture.get(), vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));

		cmd.pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eTransfer, vk::DependencyFlags(), 0, nullptr, 0,
							nullptr, 1, &barrier);

		utilities::end_single_time_cmd_buffer(cmdPool, cmd, device, physical);
	}

	/** copy buffer to image **/
	{
		auto cmd = utilities::begin_single_time_cmd_buffer(cmdPool, device);

		vk::BufferImageCopy region;
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;

		region.imageSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;

		region.imageOffset = vk::Offset3D(0, 0, 0);
		region.imageExtent = vk::Extent3D(texWidth, texHeight, 1);

		cmd.copyBufferToImage(buffer.get(), texture.get(), vk::ImageLayout::eTransferDstOptimal, 1, &region);

		utilities::end_single_time_cmd_buffer(cmdPool, cmd, device, physical);
	}


	/** transition  transferdstoptimal ==> shaderreadonlyoptimal **/
	{
		auto cmd = utilities::begin_single_time_cmd_buffer(cmdPool, device);

		vk::ImageMemoryBarrier barrier;

		// transfer dst optimal => shader read only optimal
		// required for shaders to read
		barrier = vk::ImageMemoryBarrier(vk::AccessFlagBits::eTransferWrite, vk::AccessFlagBits::eShaderRead, vk::ImageLayout::eTransferDstOptimal,
			vk::ImageLayout::eShaderReadOnlyOptimal, VK_QUEUE_FAMILY_IGNORED, VK_QUEUE_FAMILY_IGNORED,
			texture.get(), vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));


		cmd.pipelineBarrier(vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eFragmentShader, vk::DependencyFlags(), 0, nullptr, 0,
			nullptr, 1, &barrier);

		utilities::end_single_time_cmd_buffer(cmdPool, cmd, device, physical);
	}

	/** image view **/
	{
		vk::ComponentMapping componentMapping(vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA);
		vk::ImageSubresourceRange subResourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);
		vk::ImageViewCreateInfo info(vk::ImageViewCreateFlags(), texture.get(), vk::ImageViewType::e2D, vk::Format::eR8G8B8A8Unorm, componentMapping, subResourceRange);

		textureView = device.createImageViewUnique(info);
	}

	/** sampler **/
	{
		vk::SamplerCreateInfo info(vk::SamplerCreateFlags(), vk::Filter::eLinear, vk::Filter::eLinear,
									vk::SamplerMipmapMode::eLinear, vk::SamplerAddressMode::eRepeat,
									vk::SamplerAddressMode::eRepeat, vk::SamplerAddressMode::eRepeat, 0, 1, 16, 0,
									vk::CompareOp::eAlways, 0, 0, vk::BorderColor::eIntOpaqueBlack, 0);
		sampler = device.createSamplerUnique(info);
	}

	return true;
}

}
