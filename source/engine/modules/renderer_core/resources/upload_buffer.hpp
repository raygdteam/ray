#pragma once 

#include <core/debug/assert.hpp>
#include <resources/resource_manager.hpp>
#include <d3d12.h>
#include <renderer_core/renderer_core.hpp>

class RAY_RENDERERCORE_API UploadBuffer
{
	friend class RingBuffer;
	friend class UiRenderer;
private:
	u8* _begin;
	u8* _end;
	u8* _currentPointer;
	u64 _maxPoolSize;
	ID3D12Resource* _pool;

public:
	UploadBuffer() = default;
	UploadBuffer(UploadBuffer&& rhs) = default;
	UploadBuffer& operator = (UploadBuffer&& rhs) = default;

	~UploadBuffer() { Destroy(); }

public:
	void Initialize(u64 size) noexcept;
	void Destroy() noexcept;

public:
	u8* SetTextureData(RTexture& texture) noexcept;
	u8* SetBufferData(void* buffer, size_t elementsCount, size_t elementSize) noexcept;
	u8* SetConstantBufferData(void* buffer, size_t bufferSize) noexcept;

	u8* GetBeginPointer() const noexcept { return _begin; }
	ID3D12Resource* GetNativePool() const noexcept { return _pool; }
	u64 GetMaxPoolSize() const noexcept { return  _maxPoolSize; }

private:
	u8* SetBufferDataToUploadBuffer(void* buffer, size_t bufferSize, size_t alignment) noexcept;

};
