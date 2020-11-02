#include "graphics_memory_manager.hpp"
#include "renderer_core/renderer.hpp"
#include "renderer_core/d3dx12.h"

namespace ray::renderer_core_api::resources
{
	void GraphicsMemoryManager::Initialize(size_t maxTexturePoolSize, size_t maxBufferPoolSize) noexcept
	{
		CreateUploadBuffer();
		CreateTexturePool(maxTexturePoolSize);
		CreateBufferPool(maxBufferPoolSize);
	}

	void GraphicsMemoryManager::CreateUploadBuffer() noexcept
	{
		CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
		auto desc = CD3DX12_RESOURCE_DESC::Buffer(MAX_UPLOAD_BUFFER_SIZE);
		auto hr = globals::gDevice->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&_uploadBuffer));
		
		check(hr == S_OK)
		CD3DX12_RANGE range(0, 0);
		_uploadBuffer->Map(0, &range, &_beginOfUploadBuffer);
		_currentPointerOfUploadBuffer = static_cast<u8*>(_beginOfUploadBuffer);
		_endOfUploadBuffer = _currentPointerOfUploadBuffer + MAX_UPLOAD_BUFFER_SIZE;
	}

	void GraphicsMemoryManager::CreateTexturePool(size_t maxTexturePoolSize) noexcept
	{

	}

	void GraphicsMemoryManager::CreateBufferPool(size_t maxBufferPoolSize) noexcept
	{

	}

	void* GraphicsMemoryManager::AllocateFromUploadBuffer(size_t requestedSize) noexcept
	{
		ray_assert((_currentPointerOfUploadBuffer + requestedSize) <= _endOfUploadBuffer, "Out of upload buffer!")
		
		void* ret = _currentPointerOfUploadBuffer;
		_currentPointerOfUploadBuffer += requestedSize;
		return ret;
	}

	void GraphicsMemoryManager::Destroy() noexcept
	{
		if (_uploadBuffer != nullptr)
			_uploadBuffer->Release();
		if (_texturePool != nullptr)
			_texturePool->Release();
		if (_bufferPool != nullptr)
			_bufferPool->Release();
	}

}