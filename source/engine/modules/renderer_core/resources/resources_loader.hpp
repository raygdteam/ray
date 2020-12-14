#pragma once 

#include <core/core.hpp>
#include <core/debug/assert.hpp>
#include <resources/resource_manager.hpp>
#include <d3d12.h>


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
		u8* _begin;
		u8* _end;
		u8* _currentPointer;
		u64 MAX_POOL_SIZE = MB(256);
		ID3D12Resource* _underlyingResource;
	};
}