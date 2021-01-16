#include "depth_buffer.hpp"
#include <renderer_core/renderer.hpp>

DepthBuffer::DepthBuffer()
{

}

void DepthBuffer::Create(u32 width, u32 height, DXGI_FORMAT format)
{
	D3D12_RESOURCE_DESC desc = DescribeTex2D(width, height, 1, 1, format, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	D3D12_CLEAR_VALUE clearValue = {
		.Format = format,
		.DepthStencil = D3D12_DEPTH_STENCIL_VALUE {
			.Depth = 1.f,
			.Stencil = 0
		}
	};

	desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	CreateTextureResource(gDevice, desc, clearValue);
	CreateViews(format);
}

void DepthBuffer::CreateViews(DXGI_FORMAT format)
{
	D3D12_DEPTH_STENCIL_VIEW_DESC desc = {
		.Format = GetDSVFormat(format),
		.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D,
	};
	desc.Texture2D.MipSlice = 0;

	/*_dsvHandle[0] = AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	_dsvHandle[1] = AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);*/

	desc.Flags = D3D12_DSV_FLAG_NONE;
	gDevice->CreateDepthStencilView(_resource, &desc, _dsvHandle[0]);

	desc.Flags = D3D12_DSV_FLAG_READ_ONLY_DEPTH;
	gDevice->CreateDepthStencilView(_resource, &desc, _dsvHandle[1]);

	// TODO: do we need it?
	//DXGI_FORMAT stencilFormat = GetStencilFormat(format);
	//if (stencilFormat != DXGI_FORMAT_UNKNOWN)
	//{
	//	_dsvHandle[2] = globals::gDescriptorAllocator->Allocate();
	//	_dsvHandle[3] = globals::gDescriptorAllocator->Allocate();

	//	desc.Flags = D3D12_DSV_FLAG_READ_ONLY_STENCIL;
	//	globals::gDevice->CreateDepthStencilView(_resource, &desc, _dsvHandle[2]);

	//	desc.Flags = D3D12_DSV_FLAG_READ_ONLY_DEPTH | D3D12_DSV_FLAG_READ_ONLY_STENCIL;
	//	globals::gDevice->CreateDepthStencilView(_resource, &desc, _dsvHandle[3]);
	//}
	//else
	//{
	//	_dsvHandle[2] = _dsvHandle[0];
	//	_dsvHandle[3] = _dsvHandle[1];
	//}
}

