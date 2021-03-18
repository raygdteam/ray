#pragma once
#include <core/core.hpp>
#include "gpu_texture.hpp"

class GpuPixelBuffer : public GpuResource
{
protected:
	TextureView _view;

public:
	GpuPixelBuffer() = default;

public:
	void Create(GpuTextureDescription& desc, pcstr debugName) noexcept;

public:
	void Release() noexcept override;

};