#include "pch.hpp"
#include "VulkanRenderer.hpp"
#include <iostream>
#include "utils/utils.hpp"

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

		_instance = utilities::create_instance(extensions, { /*"VK_LAYER_LUNARG_core_validation"*/ });
		spdlog::trace("vulkanrenderer: created instance at @{}", (void*)&_instance.get());

		/** PHYSICAL DEVICES **/
		_physicalDevice = _instance->enumeratePhysicalDevices().front();
		auto props = _physicalDevice.getProperties();

		spdlog::trace("vulkanrenderer: physical device: {}", props.deviceName);

		/** WINDOW **/
		utilities::surface_data surface_data(_instance, _screenResolution, Platform::GetHWND());
		_surface.swap(surface_data.surface);

		_screenResolution = _physicalDevice.getSurfaceCapabilitiesKHR(_surface.get()).currentExtent;

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

		{
			_swapchainImageViews.resize(_swapchainImages.size());
			_framebuffers.resize(_swapchainImages.size());
			vk::Format colorFormat = utilities::pickSurfaceFormat(_physicalDevice.getSurfaceFormatsKHR(_surface.get())).format;
			// TODO: это не должно быть так
			vk::Format depthFormat = vk::Format::eD16Unorm;

			vk::ComponentMapping componentMapping(vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA);
			vk::ImageSubresourceRange subResourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);

			for (int i = 0; i < _swapchainImages.size(); ++i)
			{
				vk::ImageViewCreateInfo info(vk::ImageViewCreateFlags(), _swapchainImages.at(i), vk::ImageViewType::e2D, colorFormat, componentMapping, subResourceRange);
				
				_swapchainImageViews[i] = _device->createImageViewUnique(info);
			}

			for (int i = 0; i < _swapchainImages.size(); ++i)
			{
				vk::FramebufferCreateInfo cinfo(vk::FramebufferCreateFlags(), _renderPass.get(), 1,
					&_swapchainImageViews[i].get(), _screenResolution.width, _screenResolution.height, 1);
				_framebuffers[i] = _device->createFramebufferUnique(cinfo);
			}
		}

		_vertexShader.Initialize(_device, "/resources/shaders/vulkan/shader.vert", vk::ShaderStageFlagBits::eVertex);
		_fragmentShader.Initialize(_device, "/resources/shaders/vulkan/shader.frag", vk::ShaderStageFlagBits::eFragment);

		_pipelineCache = _device->createPipelineCacheUnique(vk::PipelineCacheCreateInfo());
		_pipelineLayout = _device->createPipelineLayoutUnique(vk::PipelineLayoutCreateInfo());

		vk::GraphicsPipelineCreateInfo create_info;
		std::vector<vk::PipelineShaderStageCreateInfo> stage_create_info;
		stage_create_info.resize(2);
		stage_create_info[0] = vk::PipelineShaderStageCreateInfo(vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eVertex, _vertexShader.get().get(), "main");
		stage_create_info[1] = vk::PipelineShaderStageCreateInfo(vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eFragment, _fragmentShader.get().get(), "main");

		create_info.stageCount = stage_create_info.size();
		create_info.pStages = stage_create_info.data();
		vk::PipelineVertexInputStateCreateInfo vtx_data;
		create_info.pVertexInputState = &vtx_data;
		vk::PipelineInputAssemblyStateCreateInfo imp(vk::PipelineInputAssemblyStateCreateFlags(), vk::PrimitiveTopology::eTriangleList);
		create_info.pInputAssemblyState = &imp;
		vk::PipelineViewportStateCreateInfo viewportState;
		viewportState.scissorCount = 1;
		viewportState.viewportCount = 1;
		create_info.pViewportState = &viewportState;
		vk::PipelineRasterizationStateCreateInfo rasterizationState;
		rasterizationState.lineWidth = 1.f;
		create_info.pRasterizationState = &rasterizationState;
		vk::PipelineMultisampleStateCreateInfo multisampleState;
		multisampleState.rasterizationSamples = vk::SampleCountFlagBits::e1;
		create_info.pMultisampleState = &multisampleState;
		vk::PipelineDepthStencilStateCreateInfo depthStencilState;
		create_info.pDepthStencilState = &depthStencilState;
		vk::PipelineColorBlendAttachmentState colorAttachmentState;
		colorAttachmentState.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
		vk::PipelineColorBlendStateCreateInfo colorBlendState;
		colorBlendState.attachmentCount = 1;
		colorBlendState.pAttachments = &colorAttachmentState;
		create_info.pColorBlendState = &colorBlendState;
		std::vector<vk::DynamicState> dstates = { vk::DynamicState::eViewport, vk::DynamicState::eScissor };
		vk::PipelineDynamicStateCreateInfo dynamicState;
		dynamicState.dynamicStateCount = dstates.size();
		dynamicState.pDynamicStates = dstates.data();
		create_info.pDynamicState = &dynamicState;
		create_info.layout = _pipelineLayout.get();
		create_info.renderPass = _renderPass.get();

		_pipeline = _device->createGraphicsPipelineUnique(_pipelineCache.get(), create_info);




		return true;
	}

	void VulkanRenderer::Destroy()
	{
	}

	void VulkanRenderer::Draw()
	{
		vk::ClearColorValue color(std::array<float, 4>{48.f / 255.f, 10.f / 255.f, 36.f / 255.f, 1.f});
		std::vector<vk::ClearValue> clearValue;
		clearValue.resize(2);
		clearValue[0] = vk::ClearValue(color);

		_commandBuffer->beginRenderPass(vk::RenderPassBeginInfo(_renderPass.get(), _framebuffers[_imageIndex].get(), vk::Rect2D(vk::Offset2D(), _screenResolution), 1, clearValue.data()), vk::SubpassContents::eInline);
		
		//vk::Viewport viewport = { 0, 0, (float)_screenResolution.width, (float)_screenResolution.height, 0.f, 1.f };
		vk::Viewport viewport = { 0, float(_screenResolution.height), float(_screenResolution.width), -float(_screenResolution.height), 0, 1 };
		vk::Rect2D scissor = { {0, 0}, _screenResolution };

		_commandBuffer->setViewport(0, 1, &viewport);
		_commandBuffer->setScissor(0, 1, &scissor);

		_commandBuffer->bindPipeline(vk::PipelineBindPoint::eGraphics, _pipeline.get());

		_commandBuffer->draw(3, 1, 0, 0);

		_commandBuffer->endRenderPass();
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

		_screenResolution = _physicalDevice.getSurfaceCapabilitiesKHR(_surface.get()).currentExtent;
		createSwapchain();
	}
}
