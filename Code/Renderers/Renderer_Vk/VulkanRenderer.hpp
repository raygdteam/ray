#pragma once
#include "pch.hpp"

namespace ray::renderer::vulkan
{
	class VulkanRenderer : public ray::renderer::IRenderer
	{
		vk::UniqueInstance _instance;
		vk::PhysicalDevice _physicalDevice;
		vk::UniqueDevice _device;
		vk::UniqueSurfaceKHR _surface;
		vk::UniqueSwapchainKHR _swapchain;
		vk::UniqueSemaphore _acquireSemaphore, _releaseSemaphore;
		vk::Queue _queue;
		std::vector<vk::Image> _swapchainImages;
		vk::UniqueCommandPool _commandPool;
		vk::UniqueCommandBuffer _commandBuffer;
		vk::UniqueRenderPass _renderPass;
		uint32_t _imageIndex = 0;
		uint32_t _graphicsFamilyIndex, _presentFamilyIndex;
		vk::Extent2D _screenResolution;

		bool createSwapchain();
		bool createCommandBuffers();
	public:
		VulkanRenderer();
		~VulkanRenderer();

		bool Init() override;
		void Destroy() override;
		void Draw() override;
		void BeginFrame() override;
		void EndFrame() override;

		void WindowSizeChanged(u32 width, u32 height) override;
	};
}
