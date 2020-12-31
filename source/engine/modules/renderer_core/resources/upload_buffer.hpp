#pragma once 

#include <core/core.hpp>
#include <core/debug/assert.hpp>
#include <resources/resource_manager.hpp>
#include <d3d12.h>


namespace ray::renderer_core_api::resources
{
	class UploadBuffer
	{
		friend class RingBuffer;
	public:
		void Initialize(u64 size) noexcept;
		void SetTextureData(RTexture& texture, bool bRegisterResource = true) noexcept;
		void SetBufferData(void* buffer, size_t bufferSize, size_t elementSize, bool bRegisterResource = true) noexcept;
		void SetConstantBufferData(void* buffer, size_t bufferSize, bool bRegisterResource = true) noexcept;

	private:
		void SetDataToUploadBuffer(void* buffer, size_t bufferSize, size_t alignment, bool bRegisterResource) noexcept;

	private:
		//u8* _begin;
		//u8* _end;
		//u8* _currentPointer;
		//u64 _maxPoolSize;
		//ID3D12Resource* _underlyingResource;
	};
}