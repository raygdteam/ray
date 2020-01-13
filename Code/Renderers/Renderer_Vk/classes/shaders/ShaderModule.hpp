#pragma once
#include "pch.hpp"

namespace ray::renderer::vulkan::classes::shaders
{

class ShaderModule
{
	vk::UniqueShaderModule _shader;
public:
	void Initialize(vk::UniqueDevice const& device, pcstr path, vk::ShaderStageFlagBits shaderFlags);
};

}