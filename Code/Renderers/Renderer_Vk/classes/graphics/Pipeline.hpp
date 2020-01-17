#pragma once


namespace ray::renderer::vulkan::classes::graphics
{

class Pipeline
{
public:
	virtual void Initialize(vk::UniqueDevice const&) = 0;
	virtual void Finalize(vk::UniqueDevice const&) = 0;

	virtual vk::Pipeline& GetRaw() = 0;
};

}