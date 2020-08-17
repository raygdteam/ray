#pragma once
#include "gpu_resource.hpp"
#include <string>

namespace ray::renderer_core_api::resources
{
	class PixelBuffer : public GpuResource
	{
	public:
		PixelBuffer() 
			: _width(0)
			, _height(0)
			, _arraySize(0)
			, _format(DXGI_FORMAT_UNKNOWN)
		{}

		u32 GetWidth() const noexcept { return _width; }
		u32 GetHeight() const noexcept { return _height; }
		u32 GetDepth() const noexcept { return _arraySize; }
		const DXGI_FORMAT& GetFormat() const noexcept { return _format; }

		void ExportToFile(const std::wstring& path);

	protected:
		D3D12_RESOURCE_DESC DescribeTex2D(u32 width, u32 height, u32 arraySize, u32 numMips, DXGI_FORMAT format, u32 flags);
		void AssociateWithResource(ID3D12Device* device, ID3D12Resource* resource, D3D12_RESOURCE_STATES currentState);
		void CreateTextureResource(ID3D12Device* device, const D3D12_RESOURCE_DESC& desc, D3D12_CLEAR_VALUE clearValue);

		static DXGI_FORMAT GetBaseFormat(DXGI_FORMAT format);
		static DXGI_FORMAT GetUAVFormat(DXGI_FORMAT format);
		static DXGI_FORMAT GetDSVFormat(DXGI_FORMAT format);
		static DXGI_FORMAT GetDepthFormat(DXGI_FORMAT format);
		static DXGI_FORMAT GetStencilFormat(DXGI_FORMAT format);
		static size_t BytesPerPixel(DXGI_FORMAT format);

	protected:
		u32 _width;
		u32 _height;
		u32 _arraySize;
		DXGI_FORMAT _format;

	};
}