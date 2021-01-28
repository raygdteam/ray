#pragma once
#include <core/core.hpp>
#include "gpu_texture.hpp"

class GpuPixelBufferAllocator : public GpuResourceAllocator<GpuPixelBufferMemoryPool>
{
public:
	void Initialize(size_t preferredSize) noexcept override
	{
		GpuResourceAllocator::Initialize(preferredSize);
	}

	void Destroy() noexcept override
	{
		GpuResourceAllocator::Destroy();
	}

	NODISCARD GpuResource&& Allocate(GpuResourceDescription& textureDesc) noexcept override;
	void Free(GpuResource& resource) noexcept override;

};

class GpuPixelBuffer : public GpuResource
{
	friend GpuPixelBufferAllocator;

protected:
	TextureView _view;

public:
	GpuPixelBuffer() = default;

public:
	void Create(GpuTextureDescription& desc, pcstr debugName) noexcept;

public:
	void Release() noexcept override;

};