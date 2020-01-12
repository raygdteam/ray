#pragma once
#include "pch.hpp"

namespace ray::renderer::vulkan
{
	class VulkanRenderer : public ray::renderer::IRenderer
	{
		vk::UniqueInstance _instance;
		vk::UniqueDevice _device;
		vk::UniqueSurfaceKHR _surface;
		vk::UniqueSwapchainKHR _swapchain;
		vk::UniqueSemaphore _acquireSemaphore, _releaseSemaphore;
		vk::Queue _queue;
		std::vector<vk::Image> _swapchainImages;
		vk::UniqueCommandPool _commandPool;
		vk::UniqueCommandBuffer _commandBuffer;
		uint32_t _imageIndex = 0;

	public:
		VulkanRenderer();
		~VulkanRenderer();

		bool Init() override;
		void Destroy() override;
		void Draw() override;
		void BeginFrame() override;
		void EndFrame() override;
	};
}
