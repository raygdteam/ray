#include "pch.hpp"
#include "VulkanRenderer.hpp"
#include <iostream>
#include "utils/utils.hpp"
#include "classes/texture/Texture.hpp"

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

		/* = {
			{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
			{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
		};*/
	}

	VulkanRenderer::~VulkanRenderer()
	{ }

	bool VulkanRenderer::canRender()
	{
		try
		{
			if (_screenResolution.width + _screenResolution.height == 0) return false;
			if (_physicalDevice.getSurfaceCapabilitiesKHR(_surface.get()).currentExtent.width == 0) return false;
			return true;
		}
		catch(...)
		{
			return false;
		}
	}

	bool VulkanRenderer::createSwapchain()
	{
		_swapchain = utilities::create_swapchain_khr(_physicalDevice, _device, _surface.get(), _screenResolution,
			vk::ImageUsageFlagBits::eColorAttachment,
			_graphicsFamilyIndex, _presentFamilyIndex, _swapchain.get());
		spdlog::trace("vulkanrenderer: created swapchain at @{}", (void*)& _swapchain.get());

		_swapchainImages = _device->getSwapchainImagesKHR(_swapchain.get());

		return true;
	}

	void VulkanRenderer::createSwapchainResources()
	{
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
				std::vector<vk::ImageView> attachments = { _swapchainImageViews[i].get(), _depthBuffer.imageView.get() };
				vk::FramebufferCreateInfo cinfo(vk::FramebufferCreateFlags(), _renderPass.get(), attachments.size(),
					attachments.data(), _screenResolution.width, _screenResolution.height, 1);
				_framebuffers[i] = _device->createFramebufferUnique(cinfo);
			}
		}

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
		glslang::InitializeProcess();

		_vertices.push_back(classes::vertex::Vertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)));
		_vertices.push_back(classes::vertex::Vertex(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)));
		_vertices.push_back(classes::vertex::Vertex(glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)));
		_vertices.push_back(classes::vertex::Vertex(glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)));

		_vertices.push_back(classes::vertex::Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)));
		_vertices.push_back(classes::vertex::Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)));
		_vertices.push_back(classes::vertex::Vertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)));
		_vertices.push_back(classes::vertex::Vertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)));

		_indices = {
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4
		};

		std::vector<pcstr> extensions;
		extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
#ifdef RAY_PLATFORM_WIN
		extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#endif

		_instance = utilities::create_instance(extensions, { /*"VK_LAYER_LUNARG_monitor" ,"VK_LAYER_LUNARG_core_validation", "VK_LAYER_LUNARG_parameter_validation"*/ });
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

		/** SEMAPHORES AND FENCES **/
		_acquireSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		_releaseSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		_inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
		_imagesInFlight.resize(_swapchainImages.size(), vk::Fence{});
		for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
		{
			_acquireSemaphores[i] = utilities::create_semaphore(_device);
			_releaseSemaphores[i] = utilities::create_semaphore(_device);
			_inFlightFences[i] = utilities::create_fence(_device);
		}
		
		/** QUEUE **/
		_queue = _device->getQueue(graphicsAndPresentQueueFamilyIndex.first, 0);

		/** COMMAND BUFFERS **/
		if (!createCommandBuffers()) return false;

		/** DEPTH BUFFER **/
		_depthBuffer.load(_device.get(), _physicalDevice, _commandPool.get(), _screenResolution);

		/** RENDER PASS **/
		_renderPass = utilities::create_render_pass(_physicalDevice, _surface.get(), _device, _depthBuffer.format);

		createSwapchainResources();

		_vertexShader.Initialize(_device, "/resources/shaders/vulkan/shader.vert", vk::ShaderStageFlagBits::eVertex);
		_fragmentShader.Initialize(_device, "/resources/shaders/vulkan/shader.frag", vk::ShaderStageFlagBits::eFragment);

		_texture.reset(new classes::texture::Texture);
		_texture->load("/resources/textures/wall.jpg", _device.get(), _physicalDevice, _commandPool.get());

		_graphicsPipeline.resolution = &_screenResolution;
		_graphicsPipeline.Initialize(_device);
		_graphicsPipeline.setPhysicalDevice(_physicalDevice);
		_graphicsPipeline.setSwapchainImagesSize(_swapchainImages.size());
		_graphicsPipeline.setVertexInput(_vertices);
		_graphicsPipeline.setIndexBuffer(_indices);
		_graphicsPipeline.setRenderPass(_renderPass.get());
		_graphicsPipeline.setPrimitiveTopology(vk::PrimitiveTopology::eTriangleList);
		_graphicsPipeline.setShader(_vertexShader);
		_graphicsPipeline.setShader(_fragmentShader);
		_graphicsPipeline.setTexture(_texture);
		_graphicsPipeline.Finalize(_device);


		glslang::FinalizeProcess();
		
		return true;
	}

	void VulkanRenderer::Destroy()
	{
	}

	void VulkanRenderer::Draw()
	{
		if(!canRender()) return;

		vk::ClearColorValue color(std::array<float, 4>{48.f / 255.f, 10.f / 255.f, 36.f / 255.f, 1.f});
		std::vector<vk::ClearValue> clearValue;
		clearValue.resize(2);
		clearValue[0] = vk::ClearValue(color);
		clearValue[1] = vk::ClearValue();

		clearValue[0].depthStencil = vk::ClearDepthStencilValue(0.f, 0);
		clearValue[1].color = vk::ClearColorValue(std::array<float, 4>{0.f, 0.f, 0.f, 0.f});
		clearValue[1].depthStencil = vk::ClearDepthStencilValue(1.f, 0);

		_commandBuffer->beginRenderPass(vk::RenderPassBeginInfo(_renderPass.get(), _framebuffers[_imageIndex].get(), vk::Rect2D(vk::Offset2D(), _screenResolution), clearValue.size(), clearValue.data()), vk::SubpassContents::eInline);
		
		vk::Viewport viewport = { 0, float(_screenResolution.height), float(_screenResolution.width), -float(_screenResolution.height), 0, 1 };
		vk::Rect2D scissor = { {0, 0}, _screenResolution };

		_commandBuffer->setViewport(0, 1, &viewport);
		_commandBuffer->setScissor(0, 1, &scissor);

		_graphicsPipeline.draw(_commandBuffer.get(), _imageIndex);

		_commandBuffer->endRenderPass();
	}

	void VulkanRenderer::BeginFrame()
	{
		if (!canRender()) return;

		_device->waitForFences(1, &_inFlightFences[_currentFrame].get(), true, UINT64_MAX);

		_device->acquireNextImageKHR(_swapchain.get(), UINT64_MAX, _acquireSemaphores[_currentFrame].get(), vk::Fence{}, &_imageIndex);

		bool x = (void*)_imagesInFlight[_imageIndex] != nullptr;
		if (x)
			_device->waitForFences(1, &_imagesInFlight[_imageIndex], true, UINT64_MAX);

		_imagesInFlight[_imageIndex] = _inFlightFences[_currentFrame].get();

		_device->resetCommandPool(_commandPool.get(), {});

		_commandBuffer->begin(vk::CommandBufferBeginInfo());//vk::CommandBufferUsageFlagBits::eOneTimeSubmit));
	}

	void VulkanRenderer::EndFrame()
	{
		if (!canRender()) return;

		_commandBuffer->end();

		_device->resetFences(1, &_inFlightFences[_currentFrame].get());

		vk::PipelineStageFlags flags = vk::PipelineStageFlagBits::eColorAttachmentOutput;
		vk::SubmitInfo submitInfo(1, &_acquireSemaphores[_currentFrame].get(), &flags, 1, &_commandBuffer.get(), 1, &_releaseSemaphores[_currentFrame].get());
		_queue.submit(1, &submitInfo, _inFlightFences[_currentFrame].get());

		vk::PresentInfoKHR presentInfo(1, &_releaseSemaphores[_currentFrame].get(), 1, &_swapchain.get(), &_imageIndex);
		try
		{
			assert(_queue.presentKHR(presentInfo) == vk::Result::eSuccess);
		}
		catch(vk::OutOfDateKHRError)
		{
			// This is most likely fine
		}
		
		_device->waitIdle();

		_currentFrame = (_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	void VulkanRenderer::WindowSizeChanged(u32 width, u32 height)
	{
		_device->waitIdle();

		_screenResolution = _physicalDevice.getSurfaceCapabilitiesKHR(_surface.get()).currentExtent;

		_framebuffers.clear();
		_swapchainImageViews.clear();

		_depthBuffer.destruct();
		_depthBuffer.load(_device.get(), _physicalDevice, _commandPool.get(), _screenResolution);

		createSwapchain();
		createSwapchainResources();
	}
}
