#pragma once
#include <core/core.hpp>
#include <editor/renderer/loader/volk.h>
#include <app_framework/base/platform_window.hpp>

#include <editor/renderer/render_pass.hpp>
#include <editor/renderer/pipeline.hpp>
#include <editor/renderer/shader.hpp>
#include <editor/renderer/descriptor_pool.hpp>

#define VK_SUCCEEDED(expr) ((expr) == VK_SUCCESS)
#define VK_FAILED(expr) ((expr) != VK_SUCCESS)

#undef CreateSemaphore

class IVkRenderer
{
	ray::core::IPlatformWindow* _window;
	VkExtent2D _viewport = {};
	
	VkInstance _instance = nullptr;
	VkPhysicalDevice _physicalDevice = nullptr;
	VkDevice _device = nullptr;
	VkSurfaceKHR _surface = nullptr;
	
	VkSwapchainKHR _swapchain = nullptr;
	VkImage _swapchainImages[2];
	VkImageView _swapchainImageView[2];
	VkFramebuffer _framebuffer[2];
	
	VkCommandPool _commandPool = nullptr;
	VkSemaphore _acqSemaphore = nullptr;
	VkSemaphore _relSemaphore = nullptr;
	VkQueue _queue = nullptr;

	VkCommandBuffer _cmdBuf = nullptr;

	VkRenderPass _renderPass = nullptr;
	VkPipeline _pipeline = nullptr;

	VkFormat _swapchainFormat;
	
	u32 familyIndex = 0;
	u32 _imageIdx = 0;

	bool InitInstance();
	bool InitDevice();
	bool InitSurface(ray::core::IPlatformWindow* window);
	bool InitSwapchain(ray::core::IPlatformWindow* window);
	bool InitCommandPool();
	bool InitFramebuffer();

	VkSemaphore CreateSemaphore();
	void TransitionResource(VkImage image, VkFormat format, VkImageLayout old, VkImageLayout newLayout);
	
public:
	bool Initialize(ray::core::IPlatformWindow* window);

	void BeginScene();
	void EndScene();
};
