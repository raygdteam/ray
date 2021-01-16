#include "color_buffer.hpp"
#include <renderer_core/renderer.hpp>
#include <core/debug/assert.hpp>

void ColorBuffer::CreateDerivedViews(ID3D12Device* device, DXGI_FORMAT format, u32 arraySize, u32 numMips)
{
	check(arraySize == 1 || numMips == 1);

	_numMipMaps = numMips - 1;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};

	srvDesc.Format = format;
	rtvDesc.Format = format;
	uavDesc.Format = GetUAVFormat(format);

	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	if (arraySize > 1)
	{
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
		rtvDesc.Texture2DArray.MipSlice = 0;
		rtvDesc.Texture2DArray.FirstArraySlice = 0;
		rtvDesc.Texture2DArray.ArraySize = static_cast<UINT>(arraySize);

		uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2DARRAY;
		uavDesc.Texture2DArray.MipSlice = 0;
		uavDesc.Texture2DArray.FirstArraySlice = 0;
		uavDesc.Texture2DArray.ArraySize = static_cast<UINT>(arraySize);

		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
		srvDesc.Texture2DArray.MipLevels = numMips;
		srvDesc.Texture2DArray.MostDetailedMip = 0;
		srvDesc.Texture2DArray.FirstArraySlice = 0;
		srvDesc.Texture2DArray.ArraySize = static_cast<UINT>(arraySize);
	}
	else if (_fragmentCount > 1)
	{
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DMS;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMS;
	}
	else
	{
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice = 0;

		uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
		uavDesc.Texture2D.MipSlice = 0;

		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = numMips;
		srvDesc.Texture2D.MostDetailedMip = 0;
	}

	if (_srvHandle.ptr == static_cast<D3D12_GPU_VIRTUAL_ADDRESS>(-1))
	{
		// TODO:
		/*_rtvHandle = AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		_srvHandle = AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);*/
	}

	// create the render target view
	device->CreateRenderTargetView(_resource, &rtvDesc, _rtvHandle);

	// create the shader resource view
	device->CreateShaderResourceView(_resource, &srvDesc, _srvHandle);

	if (_fragmentCount > 1)
		return;

	// create UAVs for each mip levels
	for (u32 i = 0; i < numMips; ++i)
	{
		if (_uavHandle[i].ptr == static_cast<D3D12_GPU_VIRTUAL_ADDRESS>(-1))
			/*_uavHandle[i] = AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);*/

		device->CreateUnorderedAccessView(_resource, nullptr, &uavDesc, _uavHandle[i]);

		uavDesc.Texture2D.MipSlice++;
	}
}

void ColorBuffer::CreateFromSwapChain(ID3D12Resource* inResource)
{
	AssociateWithResource(gDevice, inResource, D3D12_RESOURCE_STATE_PRESENT);
	//_rtvHandle = AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	gDevice->CreateRenderTargetView(inResource, nullptr, _rtvHandle);
}

void ColorBuffer::Create(u32 width, u32 height, u32 numMips, DXGI_FORMAT format, D3D12_GPU_VIRTUAL_ADDRESS address)
{
	numMips == (numMips == 0) ? ComputeNumMips(width, height) : numMips;
	auto flags = CombineResourceFlags();
	auto desc = DescribeTex2D(width, height, 1, numMips, format, flags);

	D3D12_CLEAR_VALUE clearValue;
	clearValue.Format = format;
	clearValue.Color[0] = _clearColor[0];
	clearValue.Color[1] = _clearColor[1];
	clearValue.Color[2] = _clearColor[2];
	clearValue.Color[3] = _clearColor[3];

	desc.SampleDesc.Count = _fragmentCount;
	desc.SampleDesc.Quality = 0;

	CreateTextureResource(gDevice, desc, clearValue);
	CreateDerivedViews(gDevice, format, 1, numMips);
}

void ColorBuffer::CreateArray(u32 width, u32 height, u32 numMips, u32 arrayCount, DXGI_FORMAT format, D3D12_GPU_VIRTUAL_ADDRESS address)
{
	auto flags = CombineResourceFlags();
	auto desc = DescribeTex2D(width, height, arrayCount, 1, format, flags);

	D3D12_CLEAR_VALUE clearValue;
	clearValue.Format = format;
	clearValue.Color[0] = _clearColor[0];
	clearValue.Color[1] = _clearColor[1];
	clearValue.Color[2] = _clearColor[2];
	clearValue.Color[3] = _clearColor[3];

	CreateTextureResource(gDevice, desc, clearValue);
	CreateDerivedViews(gDevice, format, arrayCount, 1);
}

void ColorBuffer::GenerateMipMaps(CommandContext& context)
{
	// TODO: RootSignature & ComputeContext
}
