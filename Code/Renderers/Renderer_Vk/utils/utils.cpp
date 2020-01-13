#include <pch.hpp>
#include "utils.hpp"

// #define VK_DEBUG 0

namespace ray::renderer::vulkan::utilities
{

vk::UniqueInstance create_instance(std::vector<pcstr> const& extensions,
	std::vector<pcstr> const& layers)
{
#ifdef VK_DEBUG
	{
		unsigned version = vk::enumerateInstanceVersion();
		spdlog::debug("vulkanrenderer: vulkan version {}", version);

		spdlog::debug("vulkanrenderer: available instance extensions:");

		std::vector<vk::ExtensionProperties> extension = vk::enumerateInstanceExtensionProperties();
		for (vk::ExtensionProperties ext : extension)
		{
			spdlog::debug("vulkanrenderer:  > {}", ext.extensionName);
		}

		spdlog::debug("vulkanrenderer: available instance layers:");
		std::vector<vk::LayerProperties> layer = vk::enumerateInstanceLayerProperties();
		for (vk::LayerProperties ext : layer)
		{
			spdlog::debug("vulkanrenderer:  > {}", ext.layerName);
			spdlog::debug("vulkanrenderer:  >  > {}", ext.description);
		}
	}
#endif
	vk::ApplicationInfo applicationInfo("ray_app", VK_MAKE_VERSION(0, 1, 0), "ray_engine", VK_MAKE_VERSION(0, 1, 0), VK_API_VERSION_1_1);
	vk::InstanceCreateInfo instanceCreateInfo({}, &applicationInfo);

	if (!extensions.empty())
	{
		instanceCreateInfo.ppEnabledExtensionNames = extensions.data();
		instanceCreateInfo.enabledExtensionCount = extensions.size();
	}

	if(!layers.empty())
	{
		instanceCreateInfo.ppEnabledLayerNames = layers.data();
		instanceCreateInfo.enabledLayerCount = layers.size();
	}

	try
	{
		return vk::createInstanceUnique(instanceCreateInfo);
	}
	catch (...)
	{
		spdlog::critical("vulkanrenderer: there was an error creating instance");
		return vk::UniqueInstance{};
	}
}

vk::UniqueDevice create_device(vk::PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, std::vector<pcstr> const& extensions)
{
	float queuePriority = 0.0f;
	vk::DeviceQueueCreateInfo deviceQueueCreateInfo(vk::DeviceQueueCreateFlags(), queueFamilyIndex, 1, &queuePriority);
	vk::DeviceCreateInfo deviceCreateInfo(vk::DeviceCreateFlags(),
		1,
		&deviceQueueCreateInfo,
		0,
		nullptr,
		extensions.size(),
		extensions.data());

	return physicalDevice.createDeviceUnique(deviceCreateInfo);
}

uint32_t findGraphicsQueueFamilyIndex(std::vector<vk::QueueFamilyProperties> const& queueFamilyProperties)
{
	// get the first index into queueFamiliyProperties which supports graphics
	size_t graphicsQueueFamilyIndex = std::distance(queueFamilyProperties.begin(), std::find_if(queueFamilyProperties.begin(), queueFamilyProperties.end(),
		[](vk::QueueFamilyProperties const& qfp) { return qfp.queueFlags& vk::QueueFlagBits::eGraphics; }));
	assert(graphicsQueueFamilyIndex < queueFamilyProperties.size());

	return static_cast<uint32_t>(graphicsQueueFamilyIndex);
}

std::pair<uint32_t, uint32_t> findGraphicsAndPresentQueueFamilyIndex(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR const& surface)
{
	std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();
	assert(queueFamilyProperties.size() < std::numeric_limits<uint32_t>::max());

	uint32_t graphicsQueueFamilyIndex = findGraphicsQueueFamilyIndex(queueFamilyProperties);
	if (physicalDevice.getSurfaceSupportKHR(graphicsQueueFamilyIndex, surface))
	{
		return std::make_pair(graphicsQueueFamilyIndex, graphicsQueueFamilyIndex);    // the first graphicsQueueFamilyIndex does also support presents
	}

	// the graphicsQueueFamilyIndex doesn't support present -> look for an other family index that supports both graphics and present
	for (size_t i = 0; i < queueFamilyProperties.size(); i++)
	{
		if ((queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eGraphics) && physicalDevice.getSurfaceSupportKHR(static_cast<uint32_t>(i), surface))
		{
			return std::make_pair(static_cast<uint32_t>(i), static_cast<uint32_t>(i));
		}
	}

	// there's nothing like a single family index that supports both graphics and present -> look for an other family index that supports present
	for (size_t i = 0; i < queueFamilyProperties.size(); i++)
	{
		if (physicalDevice.getSurfaceSupportKHR(static_cast<uint32_t>(i), surface))
		{
			return std::make_pair(graphicsQueueFamilyIndex, static_cast<uint32_t>(i));
		}
	}

	spdlog::critical("vulkanrenderer: could not find queues for both graphics or present");
	return std::make_pair(-1, -1);
}

vk::SurfaceFormatKHR pickSurfaceFormat(std::vector<vk::SurfaceFormatKHR> const& formats)
{
	assert(!formats.empty());
	vk::SurfaceFormatKHR pickedFormat = formats[0];
	if (formats.size() == 1)
	{
		if (formats[0].format == vk::Format::eUndefined)
		{
			pickedFormat.format = vk::Format::eB8G8R8A8Unorm;
			pickedFormat.colorSpace = vk::ColorSpaceKHR::eSrgbNonlinear;
		}
	}
	else
	{
		// request several formats, the first found will be used
		vk::Format        requestedFormats[] = { vk::Format::eB8G8R8A8Unorm, vk::Format::eR8G8B8A8Unorm, vk::Format::eB8G8R8Unorm, vk::Format::eR8G8B8Unorm };
		vk::ColorSpaceKHR requestedColorSpace = vk::ColorSpaceKHR::eSrgbNonlinear;
		for (size_t i = 0; i < sizeof(requestedFormats) / sizeof(requestedFormats[0]); i++)
		{
			vk::Format requestedFormat = requestedFormats[i];
			auto it = std::find_if(formats.begin(), formats.end(), [requestedFormat, requestedColorSpace](auto const& f) { return (f.format == requestedFormat) && (f.colorSpace == requestedColorSpace); });
			if (it != formats.end())
			{
				pickedFormat = *it;
				break;
			}
		}
	}
	assert(pickedFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear);
	return pickedFormat;
}
vk::PresentModeKHR pickPresentMode(std::vector<vk::PresentModeKHR> const& presentModes)
{
	vk::PresentModeKHR pickedMode = vk::PresentModeKHR::eFifo;
	/*for (const auto& presentMode : presentModes)
	{
		if (presentMode == vk::PresentModeKHR::eMailbox)
		{
			pickedMode = presentMode;
			break;
		}

		if (presentMode == vk::PresentModeKHR::eImmediate)
		{
			pickedMode = presentMode;
		}
	}*/
	return pickedMode;
}

swapchain_data::swapchain_data(vk::PhysicalDevice const& physicalDevice, vk::UniqueDevice const& device,
	vk::SurfaceKHR const& surface, vk::Extent2D const& extent, vk::ImageUsageFlags usage,
	uint32_t graphicsQueueFamilyIndex, uint32_t presentQueueFamilyIndex)

{
	vk::SurfaceFormatKHR surfaceFormat = pickSurfaceFormat(physicalDevice.getSurfaceFormatsKHR(surface));
	colorFormat = surfaceFormat.format;

	vk::SurfaceCapabilitiesKHR surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);
	VkExtent2D swapchainExtent;
	if (surfaceCapabilities.currentExtent.width == std::numeric_limits<uint32_t>::max())
	{
		// If the surface size is undefined, the size is set to the size of the images requested.
		swapchainExtent.width = clamp(extent.width, surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width);
		swapchainExtent.height = clamp(extent.height, surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height);
	}
	else
	{
		// If the surface size is defined, the swap chain size must match
		swapchainExtent = surfaceCapabilities.currentExtent;
	}
	vk::SurfaceTransformFlagBitsKHR preTransform = (surfaceCapabilities.supportedTransforms & vk::SurfaceTransformFlagBitsKHR::eIdentity) ? vk::SurfaceTransformFlagBitsKHR::eIdentity : surfaceCapabilities.currentTransform;
	vk::CompositeAlphaFlagBitsKHR compositeAlpha =
		(surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePreMultiplied) ? vk::CompositeAlphaFlagBitsKHR::ePreMultiplied :
		(surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePostMultiplied) ? vk::CompositeAlphaFlagBitsKHR::ePostMultiplied :
		(surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::eInherit) ? vk::CompositeAlphaFlagBitsKHR::eInherit : vk::CompositeAlphaFlagBitsKHR::eOpaque;
	vk::PresentModeKHR presentMode = pickPresentMode(physicalDevice.getSurfacePresentModesKHR(surface));
	vk::SwapchainCreateInfoKHR swapChainCreateInfo({}, surface, surfaceCapabilities.minImageCount, colorFormat, surfaceFormat.colorSpace, swapchainExtent, 1, usage, vk::SharingMode::eExclusive,
		0, nullptr, preTransform, compositeAlpha, presentMode, true);
	if (graphicsQueueFamilyIndex != presentQueueFamilyIndex)
	{
		uint32_t queueFamilyIndices[2] = { graphicsQueueFamilyIndex, presentQueueFamilyIndex };
		// If the graphics and present queues are from different queue families, we either have to explicitly transfer ownership of images between
		// the queues, or we have to create the swapchain with imageSharingMode as vk::SharingMode::eConcurrent
		swapChainCreateInfo.imageSharingMode = vk::SharingMode::eConcurrent;
		swapChainCreateInfo.queueFamilyIndexCount = 2;
		swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	swapchain = device->createSwapchainKHRUnique(swapChainCreateInfo);

	images = device->getSwapchainImagesKHR(swapchain.get());

	imageViews.reserve(images.size());
	vk::ComponentMapping componentMapping(vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA);
	vk::ImageSubresourceRange subResourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);
	for (auto image : images)
	{
		vk::ImageViewCreateInfo imageViewCreateInfo(vk::ImageViewCreateFlags(), image, vk::ImageViewType::e2D, colorFormat, componentMapping, subResourceRange);
		imageViews.push_back(device->createImageViewUnique(imageViewCreateInfo));
	}
}

surface_data::surface_data(vk::UniqueInstance& instance, vk::Extent2D const& extent, HWND hwnd) : extent(extent)
{
#if defined(VK_USE_PLATFORM_WIN32_KHR)
	surface = instance->createWin32SurfaceKHRUnique(vk::Win32SurfaceCreateInfoKHR(vk::Win32SurfaceCreateFlagsKHR(), GetModuleHandle(nullptr), hwnd));
#else
#pragma error "unhandled platform"
#endif
}


vk::UniqueSemaphore create_semaphore(vk::UniqueDevice const& device)
{
	vk::SemaphoreCreateInfo createInfo;
	auto semaphore = device->createSemaphoreUnique(createInfo);

	// TODO: это не так работает
	if(!semaphore)
	{
		spdlog::critical("vulkanrenderer: failed to create semaphore");
		return vk::UniqueSemaphore{};
	}

	spdlog::trace("vulkanrenderer: semaphore created at @{}", (void*)semaphore.get());
	return semaphore;
}

vk::UniqueSwapchainKHR create_swapchain_khr(vk::PhysicalDevice const& physicalDevice, vk::UniqueDevice const& device,
	vk::SurfaceKHR const& surface, vk::Extent2D const& extent, vk::ImageUsageFlags usage, uint32_t graphicsFamilyIndex,
	uint32_t presentFamilyIndex, vk::SwapchainKHR old)
{
	vk::SurfaceFormatKHR surfaceFormat = pickSurfaceFormat(physicalDevice.getSurfaceFormatsKHR(surface));

	vk::SurfaceCapabilitiesKHR surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);
	VkExtent2D swapchainExtent;
	if (surfaceCapabilities.currentExtent.width == std::numeric_limits<uint32_t>::max())
	{
		// If the surface size is undefined, the size is set to the size of the images requested.
		swapchainExtent.width = clamp(extent.width, surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width);
		swapchainExtent.height = clamp(extent.height, surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height);
	}
	else
	{
		// If the surface size is defined, the swap chain size must match
		swapchainExtent = surfaceCapabilities.currentExtent;
	}

	vk::SurfaceTransformFlagBitsKHR preTransform = (surfaceCapabilities.supportedTransforms & vk::SurfaceTransformFlagBitsKHR::eIdentity) ? vk::SurfaceTransformFlagBitsKHR::eIdentity : surfaceCapabilities.currentTransform;
	vk::CompositeAlphaFlagBitsKHR compositeAlpha =
		(surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePreMultiplied) ? vk::CompositeAlphaFlagBitsKHR::ePreMultiplied :
		(surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePostMultiplied) ? vk::CompositeAlphaFlagBitsKHR::ePostMultiplied :
		(surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::eInherit) ? vk::CompositeAlphaFlagBitsKHR::eInherit : vk::CompositeAlphaFlagBitsKHR::eOpaque;
	vk::PresentModeKHR presentMode = pickPresentMode(physicalDevice.getSurfacePresentModesKHR(surface));
	vk::SwapchainCreateInfoKHR swapChainCreateInfo({}, surface, surfaceCapabilities.minImageCount, surfaceFormat.format, surfaceFormat.colorSpace, swapchainExtent, 1, usage, vk::SharingMode::eExclusive,
		0, nullptr, preTransform, compositeAlpha, presentMode, true);
	if(&old != nullptr)
		swapChainCreateInfo.oldSwapchain = old;

	if (graphicsFamilyIndex != presentFamilyIndex)
	{
		uint32_t queueFamilyIndices[2] = { graphicsFamilyIndex, presentFamilyIndex };
		// If the graphics and present queues are from different queue families, we either have to explicitly transfer ownership of images between
		// the queues, or we have to create the swapchain with imageSharingMode as vk::SharingMode::eConcurrent
		swapChainCreateInfo.imageSharingMode = vk::SharingMode::eConcurrent;
		swapChainCreateInfo.queueFamilyIndexCount = 2;
		swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
	}

	return device->createSwapchainKHRUnique(swapChainCreateInfo);
}

vk::UniqueRenderPass create_render_pass(vk::PhysicalDevice const& physicalDevice, vk::SurfaceKHR const& surface, vk::UniqueDevice const& device)
{
	vk::Format colorFormat = pickSurfaceFormat(physicalDevice.getSurfaceFormatsKHR(surface)).format;
	// TODO: это не должно быть так
	vk::Format depthFormat = vk::Format::eD16Unorm;

	vk::AttachmentDescription attachmentDescriptions[2];
	attachmentDescriptions[0] = vk::AttachmentDescription(vk::AttachmentDescriptionFlags(), colorFormat, vk::SampleCountFlagBits::e1, vk::AttachmentLoadOp::eClear,
		vk::AttachmentStoreOp::eStore, vk::AttachmentLoadOp::eDontCare, vk::AttachmentStoreOp::eDontCare, vk::ImageLayout::eUndefined, vk::ImageLayout::ePresentSrcKHR);
	attachmentDescriptions[1] = vk::AttachmentDescription(vk::AttachmentDescriptionFlags(), depthFormat, vk::SampleCountFlagBits::e1, vk::AttachmentLoadOp::eClear,
		vk::AttachmentStoreOp::eDontCare, vk::AttachmentLoadOp::eDontCare, vk::AttachmentStoreOp::eDontCare, vk::ImageLayout::eUndefined, vk::ImageLayout::eDepthStencilAttachmentOptimal);

	vk::AttachmentReference colorReference(0, vk::ImageLayout::eColorAttachmentOptimal);
	vk::AttachmentReference depthReference(1, vk::ImageLayout::eDepthStencilAttachmentOptimal);
	vk::SubpassDescription subpass(vk::SubpassDescriptionFlags(), vk::PipelineBindPoint::eGraphics, 0, nullptr, 1, &colorReference, nullptr, &depthReference);

	return device->createRenderPassUnique(vk::RenderPassCreateInfo(vk::RenderPassCreateFlags(), 2, attachmentDescriptions, 1, &subpass));
}
}
