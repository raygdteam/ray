#pragma once
#include <renderer_core/resources/pixel_buffer.hpp>

namespace ray::renderer_core_api::resources
{
	class DepthBuffer : public PixelBuffer
	{
		D3D12_CPU_DESCRIPTOR_HANDLE _dsvHandle[4];
		
		void CreateViews(DXGI_FORMAT format);
	public:
		DepthBuffer();

		void Create(u32 width, u32 height, DXGI_FORMAT format);
	};
}
