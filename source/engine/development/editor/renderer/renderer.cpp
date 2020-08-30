#include <core/core.hpp>
#include <core/lib/array.hpp>
#include <core/log/log.hpp>

#include <editor/renderer/loader/volk.h>
#include <editor/renderer/renderer.hpp>

#include <windows.h>
#undef CreateSemaphore

static Logger gLog("vulkan");

bool IVkRenderer::InitInstance()
{
	if (volkInitialize() != VK_SUCCESS)
	{
		return false;
	}

	VkApplicationInfo applicationInfo = {
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pNext = nullptr,
		.pApplicationName = "RAY_ENGINE__TEST_APP",
		.applicationVersion = VK_MAKE_VERSION(0, 1, 0),
		.pEngineName = "RAY_ENGINE",
		.engineVersion = VK_MAKE_VERSION(0, 1, 0),
		.apiVersion = VK_API_VERSION_1_2
	};

	const char* layerNames[] = { "VK_LAYER_KHRONOS_validation" };
	const char* extensionNames[] =
	{
		VK_KHR_SURFACE_EXTENSION_NAME,
		VK_KHR_WIN32_SURFACE_EXTENSION_NAME
	};

	VkInstanceCreateInfo instanceCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pNext = nullptr,
		.flags = NULL,
		.pApplicationInfo = &applicationInfo,

		.enabledLayerCount = 1,
		.ppEnabledLayerNames = &layerNames[0],

		.enabledExtensionCount = 2,
		.ppEnabledExtensionNames = extensionNames,
	};

	if (VK_FAILED(vkCreateInstance(&instanceCreateInfo, nullptr, &_instance)))
	{
		return false;
	}

	volkLoadInstance(_instance);

	return true;
}

bool IVkRenderer::InitDevice()
{
	u32 physicalDeviceCount = 0;
	vkEnumeratePhysicalDevices(_instance, &physicalDeviceCount, nullptr);

	Array<VkPhysicalDevice> physicalDevices;
	physicalDevices.resize(physicalDeviceCount);

	if (VK_FAILED(vkEnumeratePhysicalDevices(_instance, &physicalDeviceCount, physicalDevices.data())))
	{
		return false;
	}
	
	for (uint32_t i = 0; i < physicalDeviceCount; ++i)
	{
		VkPhysicalDeviceProperties props;
		vkGetPhysicalDeviceProperties(physicalDevices[i], &props);

		if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		{
			gLog.Log("Picking discrete GPU: {}", props.deviceName);
			_physicalDevice = physicalDevices[i];
		}
	}

	if (physicalDeviceCount > 0 && _physicalDevice == nullptr)
	{
		VkPhysicalDeviceProperties props;
		vkGetPhysicalDeviceProperties(physicalDevices[0], &props);

		gLog.Log("Picking fallback GPU: {}", props.deviceName);
		_physicalDevice = physicalDevices[0];
	}

	familyIndex = 0; // SHORTCUT: this needs to be computed from queue properties // TODO: this produces a validation error

	float queuePriorities[] = { 1.0f };

	VkDeviceQueueCreateInfo queueInfo = {
		.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
		.pNext = nullptr,
		.queueFamilyIndex = familyIndex,
		.queueCount = 1,
		.pQueuePriorities = queuePriorities,
	};

	const char* extensions[] =
	{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	VkDeviceCreateInfo createInfo = {
		.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.pNext = nullptr,
		.queueCreateInfoCount = 1,
		.pQueueCreateInfos = &queueInfo,

		.enabledLayerCount = 0,
		.ppEnabledLayerNames = nullptr,

		.enabledExtensionCount = sizeof(extensions) / sizeof(extensions[0]),
		.ppEnabledExtensionNames = extensions,

		.pEnabledFeatures = nullptr
	};

	if (VK_FAILED(vkCreateDevice(_physicalDevice, &createInfo, nullptr, &_device)))
	{
		return false;
	}

	return true;
}

bool IVkRenderer::InitSurface(ray::core::IPlatformWindow* window)
{
	VkWin32SurfaceCreateInfoKHR createInfo = {
		.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
		.pNext = nullptr,
		.flags = NULL,
		.hinstance = GetModuleHandleA(nullptr),
		.hwnd = static_cast<HWND>(window->GetWindowHandleRaw())
	};

	if (VK_FAILED(vkCreateWin32SurfaceKHR(_instance, &createInfo, 0, &_surface)))
	{
		return false;
	}
	
	return true;
}

bool IVkRenderer::InitSwapchain(ray::core::IPlatformWindow* window)
{
	VkSwapchainCreateInfoKHR createInfo = {
		.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		.pNext = nullptr,
		.flags = NULL,
		.surface = _surface,
		.minImageCount = 2,
		.imageFormat = VK_FORMAT_R8G8B8A8_UNORM,
		.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
		.imageExtent = {
			.width = window->GetWidth(),
			.height = window->GetHeight()
		},
		.imageArrayLayers = 1,
		.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, /* attachement to pipeline */
		.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
		.queueFamilyIndexCount = 1,
		.pQueueFamilyIndices = &familyIndex,
		.preTransform = static_cast<VkSurfaceTransformFlagBitsKHR>(0),
		.compositeAlpha = static_cast<VkCompositeAlphaFlagBitsKHR>(0),
		.presentMode = VK_PRESENT_MODE_FIFO_KHR,
		.clipped = false,
		.oldSwapchain = nullptr,
	};

	if (VK_FAILED(vkCreateSwapchainKHR(_device, &createInfo, nullptr, &_swapchain)))
	{
		return false;
	}

	u32 swapchainImageCount = 16;
	vkGetSwapchainImagesKHR(_device, _swapchain, &swapchainImageCount, _swapchainImages);
	
	return true;
}

bool IVkRenderer::InitCommandPool()
{
	VkCommandPoolCreateInfo createInfo = {
		.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0,
		.queueFamilyIndex = familyIndex
	};

	if (VK_FAILED(vkCreateCommandPool(_device, &createInfo, nullptr, &_commandPool)))
	{
		return false;
	}

	return true;
}

VkSemaphore IVkRenderer::CreateSemaphore()
{
	VkSemaphoreCreateInfo createInfo = {
		.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0
	};

	VkSemaphore semaphore = nullptr;
	if (VK_FAILED(vkCreateSemaphore(_device, &createInfo, nullptr, &semaphore)))
	{
		return nullptr;
	}
	
	return semaphore;
}

bool IVkRenderer::Initialize(ray::core::IPlatformWindow* window)
{
	gLog.Log(" -------------------- BEGIN VULKAN INIT --------------------");
	if (!InitInstance()) return false;
	if (!InitDevice()) return false;
	if (!InitSurface(window)) return false;
	if (!InitSwapchain(window)) return false;
	if (!InitCommandPool()) return false;
	
	_acqSemaphore = CreateSemaphore();
	_relSemaphore = CreateSemaphore();
	
	vkGetDeviceQueue(_device, familyIndex, 0, &_queue);

	VkCommandBufferAllocateInfo allocateInfo = {
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.pNext = nullptr,
		.commandPool = _commandPool,
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = 1
	};

	vkAllocateCommandBuffers(_device, &allocateInfo, &_cmdBuf);

	gLog.Log(" --------------------- END VULKAN INIT ---------------------");
	return true;
}

void IVkRenderer::BeginScene()
{
	(vkAcquireNextImageKHR(_device, _swapchain, ~0ull, _acqSemaphore, nullptr, &_imageIdx));

	(vkResetCommandPool(_device, _commandPool, 0));

	VkCommandBufferBeginInfo beginInfo = {
		.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		.pNext = nullptr,
		.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
		.pInheritanceInfo = nullptr
	};

	(vkBeginCommandBuffer(_cmdBuf, &beginInfo));

	VkClearColorValue color = { {1, 0, 1, 1 } };

	VkImageSubresourceRange range = {
		.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
		.baseMipLevel = 0,
		.levelCount = 1,
		.baseArrayLayer = 0,
		.layerCount = 1,
	};

	vkCmdClearColorImage(_cmdBuf, _swapchainImages[_imageIdx], VK_IMAGE_LAYOUT_GENERAL, &color, 1, &range);
}

void IVkRenderer::EndScene()
{
	(vkEndCommandBuffer(_cmdBuf));

	VkPipelineStageFlags submitStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

	VkSubmitInfo submitInfo = {
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
		.pNext = nullptr,
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = &_acqSemaphore,
		.pWaitDstStageMask = &submitStageMask,
		.commandBufferCount = 1,
		.pCommandBuffers = &_cmdBuf,
		.signalSemaphoreCount = 1,
		.pSignalSemaphores = &_relSemaphore,
	};

	vkQueueSubmit(_queue, 1, &submitInfo, VK_NULL_HANDLE);

	VkPresentInfoKHR presentInfo = {
		.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
		.pNext = nullptr,
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = &_relSemaphore,
		.swapchainCount = 1,
		.pSwapchains = &_swapchain,
		.pImageIndices = &_imageIdx,
		.pResults = nullptr
	};
	(vkQueuePresentKHR(_queue, &presentInfo));

	(vkDeviceWaitIdle(_device));

}

