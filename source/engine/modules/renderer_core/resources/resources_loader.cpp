#include "resources_loader.hpp"
#include "graphics_memory_manager.hpp"
#include "resources_table.hpp"
#include "renderer_core/renderer.hpp"


#include <core/math/common.hpp>

using namespace ray::core::math;

namespace ray::renderer_core_api::resources
{

	void ResourcesLoader::Initialize() noexcept
	{

	}

	void ResourcesLoader::SetBufferData(void* buffer, size_t bufferSize, size_t elementSize) noexcept
	{
		SetDataToUploadBuffer(buffer, bufferSize, elementSize);
	}

	void ResourcesLoader::SetConstantBufferData(void* buffer, size_t bufferSize) noexcept
	{
		SetDataToUploadBuffer(buffer, bufferSize, 256);
	}

	void ResourcesLoader::SetTextureData(RTexture& texture) noexcept
	{

	}

	void ResourcesLoader::SetDataToUploadBuffer(void* buffer, size_t bufferSize, size_t alignment) noexcept
	{

	}

}