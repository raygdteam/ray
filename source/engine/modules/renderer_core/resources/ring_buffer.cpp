#include "ring_buffer.hpp"
#include "gpu_texture.hpp"

#include <core/math/common.hpp>

#include <renderer_core/renderer.hpp>
#include <renderer_core/command_queue.hpp>

void RingBuffer::Initialize(u64 maxRingBufferSize) noexcept
{
	_uploadBuffer.Initialize(maxRingBufferSize);
}

u8* RingBuffer::SetTextureData(RTexture& texture) noexcept
{
	size_t bitesPerPixel = GpuTexture::BytesPerPixel(DXGI_FORMAT_R32G32B32A32_FLOAT);
	size_t width = texture.GetDimensions().x;
	size_t height = texture.GetDimensions().y;
	size_t rowPitch = AlignUp(bitesPerPixel * width, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT);
	size_t textureSize = height * rowPitch;

	ray_assert(TryToSetResource(textureSize, D3D12_TEXTURE_DATA_PLACEMENT_ALIGNMENT), "Out of memory! Create new ring buffer!");
	u8* ret = _uploadBuffer.SetTextureData(texture);
	_frameOffsetQueue.push(FrameResourceOffset{ gCommandListManager.GetGraphicsQueue().GetNextFenceValue(), ret });
	return ret;
}

u8* RingBuffer::SetBufferData(void* buffer, size_t elementsCount, size_t elementSize) noexcept
{
	ray_assert(TryToSetResource(elementsCount * elementSize, 4), "Out of memory! Create new ring buffer!");
	u8* ret = _uploadBuffer.SetBufferData(buffer, elementsCount, elementSize);
	_frameOffsetQueue.push(FrameResourceOffset { gCommandListManager.GetGraphicsQueue().GetNextFenceValue(), ret });
	return ret;
}

u8* RingBuffer::SetConstantBufferData(void* buffer, size_t bufferSize) noexcept
{
	ray_assert(TryToSetResource(bufferSize, D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT), "Out of memory! Create new ring buffer!");
	u8* ret = _uploadBuffer.SetConstantBufferData(buffer, bufferSize);
	_frameOffsetQueue.push(FrameResourceOffset{ gCommandListManager.GetGraphicsQueue().GetNextFenceValue(), ret });
	return ret;
}

bool RingBuffer::TryToSetResource(u64 alignedSize, u64 alignment) noexcept
{
	if (!IsMemoryEnough(_uploadBuffer._begin, _uploadBuffer._end, alignedSize))
	{
		return false;
	}

	u8* currentPointer = reinterpret_cast<u8*>(AlignUp(reinterpret_cast<size_t>(_uploadBuffer._currentPointer), alignment));
	if (_frameOffsetQueue.empty())
	{
		_uploadBuffer._currentPointer = currentPointer;

		return true;
	}

	u8* nextResource = nullptr;
	auto& frontElement = _frameOffsetQueue.front();

	if (currentPointer < frontElement.ResourceOffset)
	{
		nextResource = frontElement.ResourceOffset;
	}
	else
	{
		nextResource = _uploadBuffer._end;
		if (!IsMemoryEnough(currentPointer, _uploadBuffer._end, alignedSize))
		{
			currentPointer = reinterpret_cast<u8*>(AlignUp(reinterpret_cast<size_t>(_uploadBuffer._begin), alignment));
			nextResource = frontElement.ResourceOffset;
		}
	}

	while (!IsMemoryEnough(currentPointer, nextResource, alignedSize))
	{
		gCommandListManager.GetGraphicsQueue().WaitForFence(frontElement.FrameIndex);
		u64 lastCompletedFence = gCommandListManager.GetGraphicsQueue().GetLastCompletedValue();

		TryToFreeUpMemory(lastCompletedFence);

		if (_frameOffsetQueue.IsEmpty())
		{
			currentPointer = reinterpret_cast<u8*>(AlignUp(reinterpret_cast<size_t>(_uploadBuffer._begin), alignment));
			break;
		}

		frontElement = _frameOffsetQueue.front();
		nextResource = frontElement.ResourceOffset;
		currentPointer = reinterpret_cast<u8*>(AlignUp(reinterpret_cast<size_t>(_uploadBuffer._currentPointer), alignment));
	}

	_uploadBuffer._currentPointer = currentPointer;
	return true;
}

void RingBuffer::TryToFreeUpMemory(u64 lastCompletedFrame) noexcept
{
	while (!_frameOffsetQueue.empty() && _frameOffsetQueue.front().FrameIndex <= lastCompletedFrame)
	{
		FrameResourceOffset& offset = _frameOffsetQueue.front();
		_uploadBuffer._currentPointer = offset.ResourceOffset;
		_frameOffsetQueue.pop();
	}
}

bool RingBuffer::IsMemoryEnough(u8* begin, u8* end, u64 alignedSize) const noexcept
{
	return begin + alignedSize <= end;
}
