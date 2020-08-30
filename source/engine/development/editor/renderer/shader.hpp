#pragma once
#include <core/core.hpp>
#include <editor/renderer/loader/volk.h>

class Shader
{
public:
	static VkShaderModule LoadShader(VkDevice device, pcstr path);
};
