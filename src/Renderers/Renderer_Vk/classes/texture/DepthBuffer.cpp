#include "pch.hpp"
#include "DepthBuffer.hpp"
#include "utils/utils.hpp"

namespace ray::renderer::vulkan::classes::texture
{

bool DepthBuffer::load(vk::Device device, vk::PhysicalDevice physical, vk::CommandPool cmdPool, vk::Extent2D size)
{
	auto findSupportedFormat = [](vk::PhysicalDevice physical) -> vk::Format
	{
		// Todo: the last two has stencil
		std::vector<vk::Format> candidates = {vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint};
		vk::ImageTiling tiling = vk::ImageTiling::eOptimal;
		vk::FormatFeatureFlags features = vk::FormatFeatureFlagBits::eDepthStencilAttachment;

		for (vk::Format format : candidates) 
		{
			vk::FormatProperties props = physical.getFormatProperties(format);

			if (tiling == vk::ImageTiling::eLinear && (props.linearTilingFeatures & features) == features) {
				return format;
			}
			else if (tiling == vk::ImageTiling::eOptimal && (props.optimalTilingFeatures & features) == features) {
				return format;
			}
		}

		return candidates[0];
	};

	auto hasStencilComponent = [](vk::Format format) -> bool
	{
		if (format == vk::Format::eD32SfloatS8Uint
			|| format == vk::Format::eD24UnormS8Uint) return true;
		return false;
	};

	format = findSupportedFormat(physical);

	vk::ImageCreateInfo imageInfo;
	imageInfo.imageType = vk::ImageType::e2D;
	imageInfo.extent.width = static_cast<uint32_t>(size.width);
	imageInfo.extent.height = static_cast<uint32_t>(size.height);
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = format;
	imageInfo.tiling = vk::ImageTiling::eOptimal;
	imageInfo.initialLayout = vk::ImageLayout::eUndefined;
	imageInfo.usage = vk::ImageUsageFlagBits::eDepthStencilAttachment;
	imageInfo.sharingMode = vk::SharingMode::eExclusive;
	imageInfo.samples = vk::SampleCountFlagBits::e1;

	buffer = device.createImageUnique(imageInfo);

	auto memoryRequirements = device.getImageMemoryRequirements(buffer.get());

	memory = device.allocateMemoryUnique(vk::MemoryAllocateInfo(memoryRequirements.size, utilities::findMemoryType(memoryRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal, physical)));
	device.bindImageMemory(buffer.get(), memory.get(), 0);

	vk::ComponentMapping componentMapping(vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA);
	vk::ImageSubresourceRange subResourceRange(vk::ImageAspectFlagBits::eDepth, 0, 1, 0, 1);
	vk::ImageViewCreateInfo info(vk::ImageViewCreateFlags(), buffer.get(), vk::ImageViewType::e2D, format, componentMapping, subResourceRange);

	imageView = device.createImageViewUnique(info);

	/** transition  undefined ==> depth **/
	{
		auto cmd = utilities::begin_single_time_cmd_buffer(cmdPool, device);

		vk::ImageMemoryBarrier barrier;

		// undefined => transfer dst optimal
		barrier = vk::ImageMemoryBarrier(vk::AccessFlags(),
										vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::
										eDepthStencilAttachmentWrite, vk::ImageLayout::eUndefined,
			vk::ImageLayout::eDepthStencilAttachmentOptimal, VK_QUEUE_FAMILY_IGNORED, VK_QUEUE_FAMILY_IGNORED,
			buffer.get(), vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eDepth, 0, 1, 0, 1));

		/*if (hasStencilComponent(format))
			barrier.subresourceRange.aspectMask |= vk::ImageAspectFlagBits::eStencil;
			*/
		cmd.pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eEarlyFragmentTests, vk::DependencyFlags(), 0, nullptr, 0,
			nullptr, 1, &barrier);

		utilities::end_single_time_cmd_buffer(cmdPool, cmd, device, physical);
	}

	
	return true;
}

}
