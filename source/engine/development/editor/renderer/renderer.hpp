#pragma once
#include <core/core.hpp>
#include <editor/renderer/loader/volk.h>
#include <app_framework/base/platform_window.hpp>

#define VK_SUCCEEDED(expr) ((expr) == VK_SUCCESS)
#define VK_FAILED(expr) ((expr) != VK_SUCCESS)

#undef CreateSemaphore

class IVkRenderer
{
	VkInstance _instance = nullptr;
	VkPhysicalDevice _physicalDevice = nullptr;
	VkDevice _device = nullptr;
	VkSurfaceKHR _surface = nullptr;
	VkSwapchainKHR _swapchain = nullptr;
	VkImage _swapchainImages[16];
	VkCommandPool _commandPool = nullptr;
	VkSemaphore _acqSemaphore = nullptr;
	VkSemaphore _relSemaphore = nullptr;
	VkQueue _queue = nullptr;

	VkCommandBuffer _cmdBuf = nullptr;


	u32 familyIndex = 0;
	u32 _imageIdx = 0;

	bool InitInstance();
	bool InitDevice();
	bool InitSurface(ray::core::IPlatformWindow* window);
	bool InitSwapchain(ray::core::IPlatformWindow* window);
	bool InitCommandPool();

	VkSemaphore CreateSemaphore();
	
public:
	bool Initialize(ray::core::IPlatformWindow* window);

	void BeginScene();
	void EndScene();
};
