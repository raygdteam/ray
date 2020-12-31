#pragma once

#include <core/core.hpp>
#include <core/debug/assert.hpp>
#include <resources/resource_manager.hpp>
#include <queue>
#include <d3d12.h>

namespace ray::renderer_core_api::resources
{
	class UploadBuffer;

	struct FrameResourceOffset
	{
		u64 FrameIndex;
		u8* ResourceOffset;
	};

	class RingBuffer
	{
	public:
		void Initialize() noexcept;
		void SetTextureData(RTexture& texture) noexcept;
		void SetBufferData(void* buffer, size_t bufferSize, size_t elementSize) noexcept;
		void SetConstantBufferData(void* buffer, size_t bufferSize) noexcept;

	private:
		void TryToSetResource(u64 size, u64 alignment) noexcept;
		void TryToFreeUpMemory(u64 lastCompletedFrame) const noexcept;
		bool IsMemoryEnough(u64 alignedSize) const noexcept;

	private:

	};
}