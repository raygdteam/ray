#include "depth_buffer.hpp"
#include <renderer_core/renderer.hpp>

DescriptorHeap DepthBuffer::sMainDSV_DescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE, 256);

void DepthBuffer::Create(u32 width, u32 height, DXGI_FORMAT format)
{
	D3D12_CLEAR_VALUE clearValue = 
	{
		.Format = format,
		.DepthStencil = D3D12_DEPTH_STENCIL_VALUE 
		{
			.Depth = 1.f,
			.Stencil = 0
		}
	};

	auto dsDesc = GpuTextureDescription::Texture2D(width, height, format, 1, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL | D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE);
	dsDesc.ClearValue = &clearValue;
	GpuPixelBuffer::Create(dsDesc);
	_view.Create(*this, nullptr, nullptr, &sMainDSV_DescriptorHeap);
}
