#include "ring_buffer.hpp"
#include "texture.hpp"

#include <core/math/common.hpp>

#include <renderer_core/renderer.hpp>
#include <renderer_core/command_queue.hpp>

using namespace ray::core;

namespace ray::renderer_core_api::resources
{
	void RingBuffer::Initialize(u64 maxRingBufferSize) noexcept
	{
		_uploadBuffer.Initialize(maxRingBufferSize);
	}

	u8* RingBuffer::SetTextureData(RTexture& texture) noexcept
	{
		size_t bitesPerPixel = BitesPerPixel(DXGI_FORMAT_R32G32B32A32_FLOAT);
		size_t width = texture.GetDimensions().x;
		size_t height = texture.GetDimensions().y;
		size_t rowPitch = math::AlignUp(bitesPerPixel * width, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT);
		size_t textureSize = height * rowPitch;

		ray_assert(TryToSetResource(textureSize, D3D12_TEXTURE_DATA_PLACEMENT_ALIGNMENT), "Out of memory! Create new ring buffer!");
		u8* ret = _uploadBuffer.SetTextureData(texture);
		_frameOffsetQueue.emplace(globals::gCommandListManager.GetGraphicsQueue().GetNextFenceValue(), ret);
		return ret;
	}

	u8* RingBuffer::SetBufferData(void* buffer, size_t elementsCount, size_t elementSize) noexcept
	{
		ray_assert(TryToSetResource(elementsCount * elementSize, 4), "Out of memory! Create new ring buffer!");
		u8* ret = _uploadBuffer.SetBufferData(buffer, elementsCount, elementSize);
		_frameOffsetQueue.emplace(globals::gCommandListManager.GetGraphicsQueue().GetNextFenceValue(), ret);
		return ret;
	}

	u8* RingBuffer::SetConstantBufferData(void* buffer, size_t bufferSize) noexcept
	{
		ray_assert(TryToSetResource(bufferSize, D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT), "Out of memory! Create new ring buffer!");
		u8* ret = _uploadBuffer.SetConstantBufferData(buffer, bufferSize);
		_frameOffsetQueue.emplace(globals::gCommandListManager.GetGraphicsQueue().GetNextFenceValue(), ret);
		return ret;
	}

	bool RingBuffer::TryToSetResource(u64 alignedSize, u64 alignment) noexcept
	{
		if (!IsMemoryEnough(_uploadBuffer._begin, _uploadBuffer._end, alignedSize))
		{
			return false;
		}

		u8* currentPointer = reinterpret_cast<u8*>(math::AlignUp(reinterpret_cast<size_t>(_uploadBuffer._currentPointer), alignment));
		u8* nextResource = nullptr;
		auto& frontElement = _frameOffsetQueue.front();
		
		if (currentPointer < frontElement.ResourceOffset)
		{
			nextResource = frontElement.ResourceOffset;
		}
		else
		{
			nextResource = _uploadBuffer._end;
			if(!IsMemoryEnough(currentPointer, _uploadBuffer._end, alignedSize))
			{
				currentPointer = reinterpret_cast<u8*>(math::AlignUp(reinterpret_cast<size_t>(_uploadBuffer._begin), alignment));
				nextResource = frontElement.ResourceOffset;
			}
		}

		if(!IsMemoryEnough(currentPointer, nextResource, alignedSize))
		{
			u64 lastCompletedFence = globals::gCommandListManager.GetGraphicsQueue().GetLastCompletedValue();
			TryToFreeUpMemory(lastCompletedFence);

			while (!IsMemoryEnough(currentPointer, nextResource, alignedSize) && !_frameOffsetQueue.empty())
			{
				globals::gCommandListManager.GetGraphicsQueue().WaitForFence(frontElement.FrameIndex);
				TryToFreeUpMemory(globals::gCommandListManager.GetGraphicsQueue().GetLastCompletedValue());
			}

			if (_frameOffsetQueue.empty())
			{
				currentPointer = reinterpret_cast<u8*>(math::AlignUp(reinterpret_cast<size_t>(_uploadBuffer._begin), alignment));
			}
		}

		_uploadBuffer._currentPointer = currentPointer;
		return true;
	}

	void RingBuffer::TryToFreeUpMemory(u64 lastCompletedFrame) noexcept
	{
		while (_frameOffsetQueue.front().FrameIndex <= lastCompletedFrame)
		{
			_frameOffsetQueue.pop();
		}
	}

	bool RingBuffer::IsMemoryEnough(u8* begin, u8* end, u64 alignedSize) const noexcept
	{
		return begin + alignedSize <= end;
	}
}