#pragma once 

#include <core/core.hpp>
#include <core/debug/assert.hpp>
#include <resources/resource_manager.hpp>
#include <d3d12.h>

#define RESOURCE_TYPE_MASK 0xf000000000000000
#define LOCAL_ID_MASK 0x0fffffffffffffff
#define TEXTURE_MASK 0x2000000000000000
#define BUFFER_MASK 0x1000000000000000

namespace ray::renderer_core_api::resources
{
	class ResourcesLoader
	{
	public:
		void Initialize() noexcept;
		void SetTextureData(RTexture& texture) noexcept;
		void SetBufferData(void* buffer, size_t bufferSize, size_t elementSize) noexcept;
		void SetConstantBufferData(void* buffer, size_t bufferSize) noexcept;

	private:
		void SetDataToUploadBuffer(void* buffer, size_t bufferSize, size_t alignment) noexcept;

	private:
		const size_t MAX_BUFFER_SIZE = MB(256);
		void* _begin;
		void* _end;
		u8*   _currentPointer;

		static u64 sNumTextures;
		static u64 sNumBuffers;

	};
}