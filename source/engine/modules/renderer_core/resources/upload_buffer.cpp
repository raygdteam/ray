#include "upload_buffer.hpp"
#include "graphics_memory_manager.hpp"
#include "renderer_core/renderer.hpp"
#include "gpu_allocator.hpp"

#include <core/math/common.hpp>

using namespace ray::core::math;

namespace ray::renderer_core_api::resources
{

	void UploadBuffer::Initialize(u64 size) noexcept
	{

	}

	void UploadBuffer::SetBufferData(void* buffer, size_t bufferSize, size_t elementSize, bool bRegisterResource) noexcept
	{
	}

	void UploadBuffer::SetConstantBufferData(void* buffer, size_t bufferSize, bool bRegisterResource) noexcept
	{
	}

	void UploadBuffer::SetTextureData(RTexture& texture, bool bRegisterResource) noexcept
	{

	}

	void UploadBuffer::SetDataToUploadBuffer(void* buffer, size_t bufferSize, size_t alignment, bool bRegisterResource) noexcept
	{

	}

}