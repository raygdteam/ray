#include "pch.hpp"
#include "GraphicsPipeline.hpp"

namespace ray::renderer::vulkan::classes::graphics
{

void GraphicsPipeline::Initialize(vk::UniqueDevice const& device)
{
	_device = device.get();

	_cache = device->createPipelineCacheUnique(vk::PipelineCacheCreateInfo());

}

void GraphicsPipeline::Finalize(vk::UniqueDevice const& device)
{
	_createInfo.stageCount = _pipelineShaderStageCreateInfos.size();
	_createInfo.pStages = _pipelineShaderStageCreateInfos.data();

	_vertexBuffer.Initialize(device, _physicalDevice, _vertexInput);
	_indexBuffer.Initialize(device, _physicalDevice, _indices);

	_ubos.resize(_swapChainImages);

	for (int i = 0; i < _swapChainImages; ++i)
	{
		_ubos[i] = uniform::UniformBuffer();
		_ubos[i].Initialize(device, _physicalDevice);
	}

	/** DESCRIPTOR SET LAYOUT **/
	vk::DescriptorSetLayoutBinding uboBinding(0, vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eVertex);
	_descriptorSetLayout = device->createDescriptorSetLayoutUnique(vk::DescriptorSetLayoutCreateInfo(vk::DescriptorSetLayoutCreateFlags(), 1, &uboBinding));

	/** PIPELINE LAYOUT **/
	_layout = device->createPipelineLayoutUnique(vk::PipelineLayoutCreateInfo(vk::PipelineLayoutCreateFlags(), 1, &_descriptorSetLayout.get()));

	/** DESCRIPTOR POOL **/
	vk::DescriptorPoolSize size(vk::DescriptorType::eUniformBuffer, _swapChainImages);
	_descriptorPool = _device.createDescriptorPoolUnique(vk::DescriptorPoolCreateInfo(vk::DescriptorPoolCreateFlags(vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet), _swapChainImages, 1, &size));

	/** DESCRIPTOR POOL MEMORY ALLOC **/
	std::vector<vk::DescriptorSetLayout> layouts(_swapChainImages, _descriptorSetLayout.get());
	vk::DescriptorSetAllocateInfo allocInfo = {};
	allocInfo.descriptorPool = _descriptorPool.get();
	allocInfo.descriptorSetCount = _swapChainImages;
	allocInfo.pSetLayouts = layouts.data();
	_descriptorSets = device->allocateDescriptorSetsUnique(allocInfo);

	/** SETUP DESCRIPTOR SETS **/
	for (int i = 0; i < _swapChainImages; ++i)
	{
		vk::DescriptorBufferInfo bi(_ubos[i]._buffer.get(), 0, sizeof(uniform::UniformBufferObject));
		vk::WriteDescriptorSet descriptorWrite;
		descriptorWrite.dstSet = _descriptorSets[i].get();
		descriptorWrite.dstBinding = 0;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = vk::DescriptorType::eUniformBuffer;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &bi;

		device->updateDescriptorSets(1, &descriptorWrite, 0, nullptr);
	}

	{
		vk::PipelineVertexInputStateCreateInfo vtx_data(vk::PipelineVertexInputStateCreateFlags(), 1,
			&vertex::Vertex::getBindingDescription(), 2,
			&vertex::Vertex::getAttributeDescription()[0]);
		_createInfo.pVertexInputState = &vtx_data;

		vk::PipelineInputAssemblyStateCreateInfo ia_info;
		ia_info.topology = _primitiveTopology;
		_createInfo.pInputAssemblyState = &ia_info;


		vk::PipelineViewportStateCreateInfo viewportState;
		viewportState.scissorCount = 1;
		viewportState.viewportCount = 1;
		_createInfo.pViewportState = &viewportState;

		/** RASTERIZATION **/
		vk::PipelineRasterizationStateCreateInfo rasterizationState;
		rasterizationState.lineWidth = 1.f;
		rasterizationState.cullMode = vk::CullModeFlagBits::eBack;
		rasterizationState.frontFace = vk::FrontFace::eClockwise;
		_createInfo.pRasterizationState = &rasterizationState;


		vk::PipelineMultisampleStateCreateInfo multisampleState;
		multisampleState.rasterizationSamples = vk::SampleCountFlagBits::e1;
		_createInfo.pMultisampleState = &multisampleState;


		vk::PipelineDepthStencilStateCreateInfo depthStencilState;
		_createInfo.pDepthStencilState = &depthStencilState;


		vk::PipelineColorBlendAttachmentState colorAttachmentState;
		colorAttachmentState.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;


		vk::PipelineColorBlendStateCreateInfo colorBlendState;
		colorBlendState.attachmentCount = 1;
		colorBlendState.pAttachments = &colorAttachmentState;
		_createInfo.pColorBlendState = &colorBlendState;


		std::vector<vk::DynamicState> dstates = { vk::DynamicState::eViewport, vk::DynamicState::eScissor };
		vk::PipelineDynamicStateCreateInfo dynamicState;
		dynamicState.dynamicStateCount = dstates.size();
		dynamicState.pDynamicStates = dstates.data();
		_createInfo.pDynamicState = &dynamicState;
		_createInfo.layout = _layout.get();
		_createInfo.renderPass = _renderPass;


		_pipeline = device->createGraphicsPipelineUnique(_cache.get(), _createInfo);
	}
}

void GraphicsPipeline::setPhysicalDevice(vk::PhysicalDevice device)
{
	_physicalDevice = device;
}

void GraphicsPipeline::setVertexInput(std::vector<vertex::Vertex> input)
{
	_vertexInput = input;
}

void GraphicsPipeline::setIndexBuffer(std::vector<u32> input)
{
	_indices = input;
}

void GraphicsPipeline::setShader(shaders::ShaderModule& shaderModule)
{
	_pipelineShaderStageCreateInfos.push_back(vk::PipelineShaderStageCreateInfo(
		vk::PipelineShaderStageCreateFlags(), shaderModule.GetType(), shaderModule.get().get(), "main"));
}

void GraphicsPipeline::setPrimitiveTopology(vk::PrimitiveTopology topology)
{
	_primitiveTopology = topology;
}

void GraphicsPipeline::setPolygonMode(vk::PolygonMode mode)
{
}

void GraphicsPipeline::setCullModeFlags(vk::CullModeFlags flags)
{
}

void GraphicsPipeline::setRenderPass(vk::RenderPass renderPass)
{
	_renderPass = renderPass;
}

void GraphicsPipeline::setSwapchainImagesSize(u16 size)
{
	_swapChainImages = size;
}

void GraphicsPipeline::draw(vk::CommandBuffer cmd, int imageIndex)
{
	static auto startTime = std::chrono::high_resolution_clock::now();
	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	uniform::UniformBufferObject ubo = {};
	ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(45.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.proj = glm::perspective(glm::radians(45.0f), (float)resolution->width / (float)resolution->height, 0.1f, 10.0f);

	ubo.proj[1][1] *= -1;

	void* data;
	vkMapMemory(_device, _ubos[imageIndex]._deviceMemory.get(), 0, sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
	vkUnmapMemory(_device, _ubos[imageIndex]._deviceMemory.get());


	vk::DeviceSize offsets[1] = {0};
	cmd.bindPipeline(vk::PipelineBindPoint::eGraphics, _pipeline.get());
	cmd.bindVertexBuffers(0, 1, &_vertexBuffer._buffer.get(), &offsets[0]);
	cmd.bindIndexBuffer(_indexBuffer._buffer.get(), 0, vk::IndexType::eUint32);
	cmd.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, _layout.get(), 0, 1, &_descriptorSets[imageIndex].get(), 0, nullptr);

	//cmd.draw(_vertexInput.size(), 1, 0, 0);
	cmd.drawIndexed(_indices.size(), 1, 0, 0, 0);
}

vk::Pipeline& GraphicsPipeline::GetRaw()
{
	return _pipeline.get();
}
}
