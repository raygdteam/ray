#include "gpu_pixel_buffer.hpp"
#include <renderer_core/renderer.hpp>
#include <core/debug/assert.hpp>

void GpuPixelBuffer::Create(GpuTextureDescription& desc, pcstr debugName) noexcept
{
	ray_assert((desc.Dimension != D3D12_RESOURCE_DIMENSION_BUFFER) &&
		(desc.Dimension != D3D12_RESOURCE_DIMENSION_UNKNOWN) &&
		((desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET) ||
			(desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL)),
		"Resource dimension must be texture and resource must be render target or depth stencil buffer!");

	GpuResource::Create(gPixelBufferAllocator, desc, debugName);
}

void GpuPixelBuffer::Release() noexcept
{
	gPixelBufferAllocator.Free(*this);
}
