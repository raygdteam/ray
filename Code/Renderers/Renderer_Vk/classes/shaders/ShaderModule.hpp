#pragma once
#include "pch.hpp"

namespace ray::renderer::vulkan::classes::shaders
{

class ShaderModule
{
	vk::UniqueShaderModule _shader;
	vk::ShaderStageFlagBits _shaderType;
public:
	void Initialize(vk::UniqueDevice const& device, pcstr path, vk::ShaderStageFlagBits shaderFlags);
	vk::UniqueShaderModule const& get() const
	{ return _shader; }

	vk::ShaderStageFlagBits GetType() const
	{ return _shaderType; }
};

}