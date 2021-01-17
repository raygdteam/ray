#include <core/core.hpp>
#include <core/lib/array.hpp>
#include <core/log/log.hpp>

#include <editor/renderer/loader/volk.h>
#include <editor/renderer/renderer.hpp>

#include <windows.h>
#undef CreateSemaphore

#include <editor/imgui/imgui_impl_win32.h>
#include <editor/imgui/imgui_impl_vulkan.h>

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

	//const char* layerNames[] = { "VK_LAYER_KHRONOS_validation" };
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

		.enabledLayerCount = 0, // 1,
		.ppEnabledLayerNames = nullptr, // &layerNames[0],

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
			gLog.Log("Picking discrete GPU: %s", props.deviceName);
			_physicalDevice = physicalDevices[i];
		}
	}

	if (physicalDeviceCount > 0 && _physicalDevice == nullptr)
	{
		VkPhysicalDeviceProperties props;
		vkGetPhysicalDeviceProperties(physicalDevices[0], &props);

		gLog.Log("Picking fallback GPU: %s", props.deviceName);
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

bool IVkRenderer::InitSurface(IPlatformWindow* window)
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

	VkBool32 supported = VK_FALSE;
	vkGetPhysicalDeviceSurfaceSupportKHR(_physicalDevice, familyIndex, _surface, &supported);
	
	return true;
}

bool IVkRenderer::InitSwapchain(IPlatformWindow* window)
{
	VkSurfaceCapabilitiesKHR capabilities = {};
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_physicalDevice, _surface, &capabilities);

	_viewport = capabilities.currentExtent;
	
	VkSurfaceFormatKHR surfaceFormat = {};
	u32 count = 1;
	vkGetPhysicalDeviceSurfaceFormatsKHR(_physicalDevice, _surface, &count, &surfaceFormat);
	
	VkSwapchainCreateInfoKHR createInfo = {
		.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		.pNext = nullptr,
		.flags = NULL,
		.surface = _surface,
		.minImageCount = 2,
		.imageFormat = surfaceFormat.format,
		.imageColorSpace = surfaceFormat.colorSpace,
		.imageExtent = {
			.width = capabilities.currentExtent.width,
			.height = capabilities.currentExtent.height
		},
		.imageArrayLayers = 1,
		.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, /* attachement to pipeline */
		.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
		.queueFamilyIndexCount = 1,
		.pQueueFamilyIndices = &familyIndex,
		.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
		.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		.presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR, /*VK_PRESENT_MODE_FIFO_KHR,*/
		.clipped = false,
		.oldSwapchain = _swapchain,
	};

	if (VK_FAILED(vkCreateSwapchainKHR(_device, &createInfo, nullptr, &_swapchain)))
	{
		return false;
	}

	u32 swapchainImageCount = 2;
	vkGetSwapchainImagesKHR(_device, _swapchain, &swapchainImageCount, nullptr);
	vkGetSwapchainImagesKHR(_device, _swapchain, &swapchainImageCount, _swapchainImages);

	for (u32 i = 0; i < swapchainImageCount; ++i)
	{
		VkImageViewCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = _swapchainImages[i];
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = surfaceFormat.format;
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.layerCount = 1;

		VkImageView view = nullptr;
		vkCreateImageView(_device, &createInfo, nullptr, &view);

		_swapchainImageView[i] = view;
	}
	
	_swapchainFormat = createInfo.imageFormat;
	
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

bool IVkRenderer::InitFramebuffer()
{
	for (u8 i = 0; i < 2; ++i)
	{
		VkFramebufferCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		createInfo.renderPass = _renderPass;
		createInfo.attachmentCount = 1;
		createInfo.pAttachments = &_swapchainImageView[i];
		createInfo.width = _viewport.width;
		createInfo.height = _viewport.height;
		createInfo.layers = 1;

		VkFramebuffer framebuffer = nullptr;
		vkCreateFramebuffer(_device, &createInfo, nullptr, &framebuffer);
		_framebuffer[i] = framebuffer;
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

void IVkRenderer::TransitionResource(VkImage image, VkFormat format, VkImageLayout old, VkImageLayout newLayout)
{
	// Assuming _cmdBuf is valid
	VkImageMemoryBarrier barrier = {
		.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
		.pNext = nullptr,
		.srcAccessMask = 0,
		.dstAccessMask = 0,
		.oldLayout = old,
		.newLayout = newLayout,
		.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.image = image,
		.subresourceRange = {
			.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
			.baseMipLevel = 0,
			.levelCount = 1,
			.baseArrayLayer = 0,
			.layerCount = 1
		}
	};

	VkPipelineStageFlags sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
	VkPipelineStageFlags destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;

	if (old == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) {
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		sourceStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	}
	else if (old == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR) {
		barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		barrier.dstAccessMask = 0;

		sourceStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		destinationStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	}
	else {
		__debugbreak();
	}

	vkCmdPipelineBarrier(_cmdBuf, 
		sourceStage, destinationStage, 
		VK_DEPENDENCY_BY_REGION_BIT,
		0, nullptr,
		0, nullptr,
		1, &barrier);
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
static void ImguiCallback(void* hWnd, u32 msg, u64 wParam, s64 lParam)
{
	ImGui_ImplWin32_WndProcHandler((HWND)hWnd, msg, wParam, lParam);
}

bool IVkRenderer::Initialize(IPlatformWindow* window)
{
	gLog.Log(" -------------------- BEGIN VULKAN INIT --------------------");

	_window = window;
	
	if (!InitInstance()) return false;
	if (!InitDevice()) return false;
	if (!InitSurface(window)) return false;
	if (!InitSwapchain(window)) return false;
	if (!InitCommandPool()) return false;
	
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(window->GetWindowHandleRaw());
	
	_acqSemaphore = CreateSemaphore();
	_relSemaphore = CreateSemaphore();

	_renderPass = RenderPass::CreateRenderPass(_device, _swapchainFormat);
	_pipeline = Pipeline::CreateGraphicsPipeline(_device, _renderPass,
	                                             Shader::LoadShader(_device, "../../engine/resources/triangle.vert.spv"),
	                                             Shader::LoadShader(_device, "../../engine/resources/triangle.frag.spv"),
	                                             Pipeline::CreatePipelineLayout(_device));

	InitFramebuffer();
	
	vkGetDeviceQueue(_device, familyIndex, 0, &_queue);

	ImGui_ImplVulkan_InitInfo initInfo = {
		.Instance = _instance,
		.PhysicalDevice = _physicalDevice,
		.Device = _device,
		.QueueFamily = familyIndex,
		.Queue = _queue,
		.PipelineCache = nullptr,
		.DescriptorPool = DescriptorPool::CreateDescriptorPool(_device),
		.MinImageCount = 2,
		.ImageCount = 2,
		.MSAASamples = VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT,
		.Allocator = nullptr,
		.CheckVkResultFn = nullptr
	};
	ImGui_ImplVulkan_Init(&initInfo, _renderPass);
	

	VkCommandBufferAllocateInfo allocateInfo = {
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.pNext = nullptr,
		.commandPool = _commandPool,
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = 1
	};

	vkAllocateCommandBuffers(_device, &allocateInfo, &_cmdBuf);

	// Upload fonts(imgui)
	vkResetCommandPool(_device, _commandPool, 0);
	VkCommandBufferBeginInfo beginInfo = {
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.pNext = nullptr,
		.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
		.pInheritanceInfo = nullptr
	};

	vkBeginCommandBuffer(_cmdBuf, &beginInfo);
	ImGui_ImplVulkan_CreateFontsTexture(_cmdBuf);

	vkEndCommandBuffer(_cmdBuf);
	VkSubmitInfo submitInfo = {
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
		.pNext = nullptr,
		.waitSemaphoreCount = 0,
		.pWaitSemaphores = nullptr,
		.pWaitDstStageMask = nullptr,
		.commandBufferCount = 1,
		.pCommandBuffers = &_cmdBuf,
		.signalSemaphoreCount = 0,
		.pSignalSemaphores = nullptr,
	};
	vkQueueSubmit(_queue, 1, &submitInfo, nullptr);
	vkDeviceWaitIdle(_device);
	
	ImGui_ImplVulkan_DestroyFontUploadObjects();

	window->RegisterEventCallback(ImguiCallback);

	gLog.Log(" --------------------- END VULKAN INIT ---------------------");
	return true;
}

void IVkRenderer::BeginScene()
{
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	// ImGui::ShowDemoWindow();

	vkDeviceWaitIdle(_device);

	vkAcquireNextImageKHR(_device, _swapchain, ~0ull, _acqSemaphore, nullptr, &_imageIdx);
	
	vkResetCommandPool(_device, _commandPool, 0);

	VkCommandBufferBeginInfo beginInfo = {
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.pNext = nullptr,
		.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
		.pInheritanceInfo = nullptr
	};

	vkBeginCommandBuffer(_cmdBuf, &beginInfo);

	TransitionResource(_swapchainImages[_imageIdx], _swapchainFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
	// ImGui_ImplVulkan_NewFrame(_cmdBuf);

	VkClearColorValue color = { { 48.f / 255.f, 10.f / 255.f, 36.f / 255.f, 1 }};
	VkClearValue clearColor = { color };
	
	VkRenderPassBeginInfo passBeginInfo = {
		.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
		.pNext = nullptr,
		.renderPass = _renderPass,
		.framebuffer = _framebuffer[_imageIdx],
		.renderArea = VkRect2D {
			.extent = {
				.width = _viewport.width,
				.height = _viewport.height
			},
		},
		.clearValueCount = 1,
		.pClearValues = &clearColor,
	};

	vkCmdBeginRenderPass(_cmdBuf, &passBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

	VkViewport viewport = { 0, 0.f, float(_viewport.width), float(_viewport.height), 0, 1 };
	VkRect2D scissor = { {0, 0}, {uint32_t(_viewport.width), uint32_t(_viewport.height)} };

	vkCmdSetViewport(_cmdBuf, 0, 1, &viewport);
	vkCmdSetScissor(_cmdBuf, 0, 1, &scissor);

	/* NOTE: pipeline bind */
	vkCmdBindPipeline(_cmdBuf, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline);
	vkCmdDraw(_cmdBuf, 3, 1, 0, 0);
}

void IVkRenderer::EndScene()
{
	ImGui::EndFrame();
	ImGui::Render();
	ImGui::UpdatePlatformWindows();
	ImDrawData* drawData = ImGui::GetDrawData();

	ImGui_ImplVulkan_RenderDrawData(drawData, _cmdBuf);
	vkCmdEndRenderPass(_cmdBuf);

	TransitionResource(_swapchainImages[_imageIdx], _swapchainFormat, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

	vkEndCommandBuffer(_cmdBuf);

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
	VkResult res = vkQueuePresentKHR(_queue, &presentInfo);
	if (res == VK_ERROR_OUT_OF_DATE_KHR)
	{
		InitSwapchain(_window);
		InitFramebuffer();
	}
}
