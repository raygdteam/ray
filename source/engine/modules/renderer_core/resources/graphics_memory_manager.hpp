#pragma once 

#include <core/core.hpp>
#include <core/debug/assert.hpp>
#include <d3d12.h>

namespace ray::renderer_core_api::resources
{
	class GraphicsMemoryManager
	{
	public:
		void Initialize(size_t maxTexturePoolSize = MB(128), size_t maxBufferPoolSize = MB(128)) noexcept;

		void* AllocateFromUploadBuffer(size_t requestedSize) noexcept;
		void AllocateFromTexturePool() noexcept {}
		void AllocateFromBufferPool() noexcept {}

		void Destroy() noexcept;

	private:
		void CreateUploadBuffer() noexcept;
		void CreateTexturePool(size_t maxTexturePoolSize) noexcept;
		void CreateBufferPool(size_t maxBufferPoolSize) noexcept;

	private:
		ID3D12Resource* _uploadBuffer; // ResourcesLoader and RingBuffer use this buffer
		ID3D12Resource* _texturePool; // may be ID3D12Heap?
		ID3D12Resource* _bufferPool; // may be ID3D12Heap?
		
		/*	
			256 MB for ResourcesLoader, 
			64 MB for RingBuffer and 
			8 MB reserved 
		*/

		void* _beginOfUploadBuffer;
		u8*	  _currentPointerOfUploadBuffer;
		void* _endOfUploadBuffer;

		const size_t MAX_UPLOAD_BUFFER_SIZE = MB(256 + 64 + 8); 
		size_t _maxTexturePoolSize; // determined in run-time
		size_t _maxBufferPoolSize; // determined in run-time

	};
}