#include "gpu_pixel_buffer.hpp"
#include <renderer_core/renderer.hpp>
#include <core/debug/assert.hpp>

void GpuPixelBuffer::Create(GpuTextureDescription& desc, pcstr debugName) noexcept
{
	ray_assert((desc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE1D  ||
				desc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D  ||
				desc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE3D) &&
			  ((desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET) ||
			   (desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL)),
				"Resource dimension must be texture and resource must be render target or depth stencil buffer!")

	*dynamic_cast<GpuResource*>(this) = std::move(gPixelBufferAllocator.Allocate(desc));

#if defined(RAY_DEBUG) || defined(RAY_DEVELOPMENT)
	size_t debugNameSize = strlen(debugName);
	WCHAR dest[128];
	MultiByteToWideChar(0, 0, debugName, debugNameSize, dest, debugNameSize);
	dest[debugNameSize] = '\0';
	_resource->SetName(dest);
#endif

	// TODO
}

void GpuPixelBuffer::Release() noexcept
{
	gPixelBufferAllocator.Free(*this);
}
