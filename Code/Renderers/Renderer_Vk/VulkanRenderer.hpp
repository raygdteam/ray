#pragma once
#include "pch.hpp"
#include "classes/shaders/ShaderModule.hpp"
#include "classes/graphics/GraphicsPipeline.hpp"

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
		std::vector<vk::UniqueImageView> _swapchainImageViews;
		std::vector<vk::UniqueFramebuffer> _framebuffers;
		vk::UniqueCommandPool _commandPool;
		vk::UniqueCommandBuffer _commandBuffer;
		vk::UniqueRenderPass _renderPass;
		classes::graphics::GraphicsPipeline _graphicsPipeline;
		classes::shaders::ShaderModule _vertexShader, _fragmentShader;
		
		uint32_t _imageIndex = 0;
		uint32_t _graphicsFamilyIndex, _presentFamilyIndex;
		vk::Extent2D _screenResolution;

		std::vector<classes::vertex::Vertex> _vertices;
		std::vector<u32> _indices = {
			0, 1, 2, 2, 3, 0
		};

		std::shared_ptr<classes::texture::Texture> _texture;

		bool createSwapchain();
		void createSwapchainResources();
		bool createCommandBuffers();
		bool canRender();

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
