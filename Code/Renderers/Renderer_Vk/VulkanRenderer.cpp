#include "pch.hpp"
#include "VulkanRenderer.hpp"
#include <iostream>
#include "utils/utils.hpp"
#include "classes/shaders/ShaderModule.hpp"

VkBool32 debugUtilsMessengerCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes,
	VkDebugUtilsMessengerCallbackDataEXT const* pCallbackData, void* /*pUserData*/)
{
	std::cerr << vk::to_string(static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>(messageSeverity)) << ": " << vk::to_string(static_cast<vk::DebugUtilsMessageTypeFlagsEXT>(messageTypes)) << ":\n";
	std::cerr << "\t" << "messageIDName   = <" << pCallbackData->pMessageIdName << ">\n";
	std::cerr << "\t" << "messageIdNumber = " << pCallbackData->messageIdNumber << "\n";
	std::cerr << "\t" << "message         = <" << pCallbackData->pMessage << ">\n";
	if (0 < pCallbackData->queueLabelCount)
	{
		std::cerr << "\t" << "Queue Labels:\n";
		for (uint8_t i = 0; i < pCallbackData->queueLabelCount; i++)
		{
			std::cerr << "\t\t" << "labelName = <" << pCallbackData->pQueueLabels[i].pLabelName << ">\n";
		}
	}
	if (0 < pCallbackData->cmdBufLabelCount)
	{
		std::cerr << "\t" << "CommandBuffer Labels:\n";
		for (uint8_t i = 0; i < pCallbackData->cmdBufLabelCount; i++)
		{
			std::cerr << "\t\t" << "labelName = <" << pCallbackData->pCmdBufLabels[i].pLabelName << ">\n";
		}
	}
	if (0 < pCallbackData->objectCount)
	{
		std::cerr << "\t" << "Objects:\n";
		for (uint8_t i = 0; i < pCallbackData->objectCount; i++)
		{
			std::cerr << "\t\t" << "Object " << i << "\n";
			std::cerr << "\t\t\t" << "objectType   = " << vk::to_string(static_cast<vk::ObjectType>(pCallbackData->pObjects[i].objectType)) << "\n";
			std::cerr << "\t\t\t" << "objectHandle = " << pCallbackData->pObjects[i].objectHandle << "\n";
			if (pCallbackData->pObjects[i].pObjectName)
			{
				std::cerr << "\t\t\t" << "objectName   = <" << pCallbackData->pObjects[i].pObjectName << ">\n";
			}
		}
	}
	return VK_TRUE;
}

namespace ray::renderer::vulkan
{
	VulkanRenderer::VulkanRenderer()
	{
		_screenResolution = vk::Extent2D(Platform::GetWidth(), Platform::GetHeight());
	}

	VulkanRenderer::~VulkanRenderer()
	{ }

	bool VulkanRenderer::createSwapchain()
	{
		_swapchain = utilities::create_swapchain_khr(_physicalDevice, _device, _surface.get(), _screenResolution,
			vk::ImageUsageFlagBits::eColorAttachment,
			_graphicsFamilyIndex, _presentFamilyIndex, _swapchain.get());
		spdlog::trace("vulkanrenderer: created swapchain at @{}", (void*)& _swapchain.get());

		_swapchainImages = _device->getSwapchainImagesKHR(_swapchain.get());
		return true;
	}

	bool VulkanRenderer::createCommandBuffers()
	{
		/** COMMAND POOL **/
		vk::CommandPoolCreateInfo cpCreateInfo;
		cpCreateInfo.setFlags(vk::CommandPoolCreateFlagBits::eTransient)
			.setQueueFamilyIndex(_graphicsFamilyIndex);
		_commandPool = _device->createCommandPoolUnique(cpCreateInfo);

		/** COMMAND BUFFER **/
		_commandBuffer.swap(_device->allocateCommandBuffersUnique(vk::CommandBufferAllocateInfo(_commandPool.get(), vk::CommandBufferLevel::ePrimary, 1))[0]);

		return true;
	}

	bool VulkanRenderer::Init()
	{
		spdlog::trace("vulkanrenderer: initializing...");

		std::vector<pcstr> extensions;
		extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
#ifdef RAY_PLATFORM_WIN
		extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#endif

		_instance = utilities::create_instance(extensions, {});
		spdlog::trace("vulkanrenderer: created instance at @{}", (void*)&_instance.get());

		/** PHYSICAL DEVICES **/
		_physicalDevice = _instance->enumeratePhysicalDevices().front();
		auto props = _physicalDevice.getProperties();

		spdlog::trace("vulkanrenderer: physical device: {}", props.deviceName);

		/** WINDOW **/
		utilities::surface_data surface_data(_instance, _screenResolution, Platform::GetHWND());
		_surface.swap(surface_data.surface);

		/** GRAPHICS AND PRESENT QUEUE FAMILY INDEXES **/
		std::pair<uint32_t, uint32_t> graphicsAndPresentQueueFamilyIndex = utilities::findGraphicsAndPresentQueueFamilyIndex(_physicalDevice, _surface.get());
		_graphicsFamilyIndex = graphicsAndPresentQueueFamilyIndex.first;
		_presentFamilyIndex = graphicsAndPresentQueueFamilyIndex.second;
		spdlog::trace("vulkanrenderer: graphics queue family index = {}", _graphicsFamilyIndex);
		spdlog::trace("vulkanrenderer: present queue family index = {}", _presentFamilyIndex);

		/** LOGICAL DEVICE **/
		_device = utilities::create_device(_physicalDevice, graphicsAndPresentQueueFamilyIndex.first, { VK_KHR_SWAPCHAIN_EXTENSION_NAME });
		spdlog::trace("vulkanrenderer: created device at @{}", (void*)&_device.get());

		/** SWAPCHAIN **/
		if (!createSwapchain()) return false;

		/** SEMAPHORE **/
		_acquireSemaphore = utilities::create_semaphore(_device);
		_releaseSemaphore = utilities::create_semaphore(_device);

		/** QUEUE **/
		_queue = _device->getQueue(graphicsAndPresentQueueFamilyIndex.first, 0);

		/** COMMAND BUFFERS **/
		if (!createCommandBuffers()) return false;

		/** RENDER PASS **/
		_renderPass = utilities::create_render_pass(_physicalDevice, _surface.get(), _device);

		classes::shaders::ShaderModule shader;
		shader.Initialize(_device, "/resources/shaders/vulkan/shader.vert", vk::ShaderStageFlagBits::eVertex);

		return true;
	}

	void VulkanRenderer::Destroy()
	{
	}

	void VulkanRenderer::Draw()
	{
		vk::ClearColorValue color(std::array<float, 4>{0.0f, 1.0f, 0.0f, 1.0f});
		vk::ImageSubresourceRange image_subresource_range(vk::ImageAspectFlagBits::eColor);
		image_subresource_range.layerCount = 1;
		image_subresource_range.levelCount = 1;

		_commandBuffer->clearColorImage(_swapchainImages[_imageIndex], vk::ImageLayout::eGeneral, &color, 1, &image_subresource_range);
	}

	void VulkanRenderer::BeginFrame()
	{
		_device->acquireNextImageKHR(_swapchain.get(), ~0ull, _acquireSemaphore.get(), vk::Fence{}, &_imageIndex);

		_device->resetCommandPool(_commandPool.get(), {});

		_commandBuffer->begin(vk::CommandBufferBeginInfo(vk::CommandBufferUsageFlagBits::eOneTimeSubmit));
	}

	void VulkanRenderer::EndFrame()
	{
		_commandBuffer->end();

		vk::PipelineStageFlags flags = vk::PipelineStageFlagBits::eColorAttachmentOutput;
		vk::SubmitInfo submitInfo(1, &_acquireSemaphore.get(), &flags, 1, &_commandBuffer.get(), 1, &_releaseSemaphore.get());
		_queue.submit(1, &submitInfo, vk::Fence{});

		vk::PresentInfoKHR presentInfo(1, &_releaseSemaphore.get(), 1, &_swapchain.get(), &_imageIndex);
		assert(_queue.presentKHR(presentInfo) == vk::Result::eSuccess);
		_device->waitIdle();
	}

	void VulkanRenderer::WindowSizeChanged(u32 width, u32 height)
	{
		_device->waitIdle();

		_screenResolution = vk::Extent2D(width, height);
		createSwapchain();
	}
}
