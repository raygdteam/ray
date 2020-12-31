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
		_uploadBuffer.Initialize(MB(64));
	}

	void RingBuffer::SetTextureData(RTexture& texture) noexcept
	{
		_uploadBuffer.SetTextureData(texture, false);
	}

	void RingBuffer::SetBufferData(void* buffer, size_t bufferSize, size_t elementSize) noexcept
	{
		_uploadBuffer.SetBufferData(buffer, bufferSize, elementSize, false);
	}

	void RingBuffer::SetConstantBufferData(void* buffer, size_t bufferSize) noexcept
	{
		_uploadBuffer.SetConstantBufferData(buffer, bufferSize, false);
	}

	void RingBuffer::TryToSetResource(u64 size, u64 alignment) noexcept
	{
		
		u64 alignedSize = math::AlignUp(size, alignment);
		if (!IsMemoryEnough(alignedSize))
		{
			globals::gCommandListManager.GetQueue(D3D12_COMMAND_LIST_TYPE_DIRECT);
			u64 lastCompletedFrame = globals::gCommandListManager.GetGraphicsQueue().GetLastCompletedValue();
			TryToFreeUpMemory(lastCompletedFrame);
		}
	}

	void RingBuffer::TryToFreeUpMemory(u64 lastCompletedFrame) const noexcept
	{
		while (!_resources.empty() && _resources.front().FrameIndex <= lastCompletedFrame)
			_resources.pop();
	}

	bool RingBuffer::IsMemoryEnough(u64 alignedSize) const noexcept
	{
		return _uploadBuffer._currentPointer + alignedSize <= _uploadBuffer._end;
	}
}