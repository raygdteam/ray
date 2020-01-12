#include <pch.hpp>

namespace ray::renderer::vulkan::utilities
{

template<typename T>
T clamp(T original, T min, T max)
{
	if (original < min) original = min;
	if (original > max) original = max;
	return original;
}

vk::UniqueInstance create_instance(std::vector<pcstr> const& extensions, std::vector<pcstr> const& layers);
vk::UniqueDevice create_device(vk::PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, std::vector<pcstr> const& extensions);
std::pair<uint32_t, uint32_t> findGraphicsAndPresentQueueFamilyIndex(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR const& surface);
vk::UniqueSemaphore create_semaphore(vk::UniqueDevice const& device);
vk::UniqueSwapchainKHR create_swapchain_khr(vk::PhysicalDevice const& physicalDevice, vk::UniqueDevice const& device, vk::SurfaceKHR const& surface, vk::Extent2D const& extent, vk::ImageUsageFlags usage,
	uint32_t graphicsFamilyIndex, uint32_t presentFamilyIndex);

struct swapchain_data
{
	swapchain_data(vk::PhysicalDevice const& physicalDevice, vk::UniqueDevice const& device, vk::SurfaceKHR const& surface, vk::Extent2D const& extent, vk::ImageUsageFlags usage,
		uint32_t graphicsFamilyIndex, uint32_t presentFamilyIndex);

	vk::Format                        colorFormat;
	vk::UniqueSwapchainKHR            swapchain;
	std::vector<vk::Image>            images;
	std::vector<vk::UniqueImageView>  imageViews;
};

struct surface_data
{
	surface_data(vk::UniqueInstance& instance, vk::Extent2D const& extent, HWND hw);

	vk::Extent2D          extent;
	vk::UniqueSurfaceKHR  surface;
};

}
