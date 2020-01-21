#pragma once
#include "pch.hpp"
#include "Pipeline.hpp"
#include "classes/vertex/Vertex.hpp"
#include "classes/shaders/ShaderModule.hpp"
#include "classes/vertex/VertexBuffer.hpp"
#include "classes/vertex/IndexBuffer.hpp"
#include "classes/uniform/UniformBuffer.hpp"
#include "classes/texture/Texture.hpp"
#include "classes/texture/DepthBuffer.hpp"

namespace ray::renderer::vulkan::classes::graphics
{

class GraphicsPipeline : public Pipeline
{
	vk::PhysicalDevice _physicalDevice;
	vk::Device _device;
	vk::UniqueDescriptorSetLayout _descriptorSetLayout;
	vk::UniqueDescriptorPool _descriptorPool;
	vk::UniquePipeline _pipeline;
	vk::UniquePipelineLayout _layout;
	vk::UniquePipelineCache _cache;
	vk::RenderPass _renderPass;

	std::vector<vk::UniqueDescriptorSet> _descriptorSets;

	vertex::VertexBuffer _vertexBuffer;
	vertex::IndexBuffer _indexBuffer;

	vk::GraphicsPipelineCreateInfo _createInfo;
	std::vector<vk::PipelineShaderStageCreateInfo> _pipelineShaderStageCreateInfos;

	vk::PrimitiveTopology _primitiveTopology;
	std::vector<vertex::Vertex> _vertexInput;
	std::vector<u32> _indices;
	std::vector<uniform::UniformBuffer> _ubos;

	std::shared_ptr<texture::Texture> _texture;

	u16 _swapChainImages;

public:
	vk::Extent2D* resolution;

	void Initialize(vk::UniqueDevice const&) override;
	void Finalize(vk::UniqueDevice const&) override;

	void setPhysicalDevice(vk::PhysicalDevice);
	void setVertexInput(std::vector<vertex::Vertex> input);
	void setIndexBuffer(std::vector<u32> input);
	void setShader(shaders::ShaderModule& shaderModule);
	void setPrimitiveTopology(vk::PrimitiveTopology topology);
	void setPolygonMode(vk::PolygonMode mode);
	void setCullModeFlags(vk::CullModeFlags flags);
	void setRenderPass(vk::RenderPass renderPass);
	void setSwapchainImagesSize(u16 size);
	void setTexture(std::shared_ptr<texture::Texture> texture);

	void draw(vk::CommandBuffer, int);

	vk::Pipeline& GetRaw() override;
};
}
