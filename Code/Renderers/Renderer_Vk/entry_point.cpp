#include "pch.hpp"
#include "VulkanRenderer.hpp"

namespace ray::renderer::detail::vk
{

bool is_supported()
{
	return true;
}

IRenderer* create()
{
	return new renderer::vulkan::VulkanRenderer;
}

}
