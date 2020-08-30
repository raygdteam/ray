#pragma once
#include <core/core.hpp>
#include <editor/renderer/loader/volk.h>

class Pipeline
{
public:
	// Temp
	static VkPipelineLayout CreatePipelineLayout(VkDevice device);
	static VkPipeline CreateGraphicsPipeline(VkDevice device, VkRenderPass renderPass, VkShaderModule vs, VkShaderModule fs, VkPipelineLayout layout);
};
