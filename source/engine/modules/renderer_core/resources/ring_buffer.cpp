#include "ring_buffer.hpp"
#include "upload_buffer.hpp"

#include <core/math/common.hpp>

#include <renderer_core/renderer.hpp>
#include <renderer_core/command_queue.hpp>

using namespace ray::core;

namespace ray::renderer_core_api::resources
{
	void RingBuffer::Initialize() noexcept
	{
	}

	void RingBuffer::SetTextureData(RTexture& texture) noexcept
	{
	}

	void RingBuffer::SetBufferData(void* buffer, size_t bufferSize, size_t elementSize) noexcept
	{
	}

	void RingBuffer::SetConstantBufferData(void* buffer, size_t bufferSize) noexcept
	{
	}

	void RingBuffer::TryToSetResource(u64 size, u64 alignment) noexcept
	{
		
	}

	void RingBuffer::TryToFreeUpMemory(u64 lastCompletedFrame) const noexcept
	{
		
	}

	bool RingBuffer::IsMemoryEnough(u64 alignedSize) const noexcept
	{
		return false;
	}
}