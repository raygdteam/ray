#pragma once

#include <core/core.hpp>
#include <core/debug/assert.hpp>
#include <resources/resource_manager.hpp>
#include "upload_buffer.hpp"
#include <queue>
#include <d3d12.h>

namespace ray::renderer_core_api::resources
{

	struct FrameResourceOffset
	{
	public:
		u64 FrameIndex;
		u8* ResourceOffset;

	public:
		FrameResourceOffset(u64 frameIndex, u8* resourceOffset)
			: FrameIndex(frameIndex)
			, ResourceOffset(resourceOffset)
		{}

		FrameResourceOffset(FrameResourceOffset&& rhs) = default;
		FrameResourceOffset& operator = (FrameResourceOffset&& rhs) = default;

		FrameResourceOffset(const FrameResourceOffset& lhs) = default;
		FrameResourceOffset& operator = (const FrameResourceOffset& lhs) = default;
	};

	class RingBuffer
	{
	private:
		UploadBuffer _uploadBuffer;
		std::queue<FrameResourceOffset> _frameOffsetQueue;

	public:
		RingBuffer() = default;
		RingBuffer(RingBuffer&& rhs) = default;
		RingBuffer& operator = (RingBuffer&& rhs) = default;

		~RingBuffer() { Destroy(); }

	public:
		void Initialize(u64 maxRingBufferSize) noexcept;
		void Destroy() noexcept { _uploadBuffer.Destroy(); }

	public:
		u8* SetTextureData(RTexture& texture) noexcept;
		u8* SetBufferData(void* buffer, size_t elementsCount, size_t elementSize) noexcept;
		u8* SetConstantBufferData(void* buffer, size_t bufferSize) noexcept;

	private:
		bool TryToSetResource(u64 alignedSize, u64 alignment) noexcept;
		void TryToFreeUpMemory(u64 lastCompletedFrame) noexcept;

	private:
		bool IsMemoryEnough(u8* begin, u8* end, u64 alignedSize) const noexcept;

	};
}