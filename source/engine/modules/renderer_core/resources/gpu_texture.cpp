#include "gpu_texture.hpp"
#include <renderer_core/renderer.hpp>
#include <renderer_core/renderer_2d.hpp>
#include <renderer_core/command_context.hpp>
#include "upload_buffer.hpp"
#include <renderer_core/dx12_helper_functions.hpp>
#include <core/debug/assert.hpp>
#include <core/math/common.hpp>

DXGI_FORMAT GpuTexture::GetBaseFormat(DXGI_FORMAT defaultFormat)
{
	switch (defaultFormat)
	{
	case DXGI_FORMAT_R8G8B8A8_UNORM:
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
		return DXGI_FORMAT_R8G8B8A8_TYPELESS;

	case DXGI_FORMAT_B8G8R8A8_UNORM:
	case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
		return DXGI_FORMAT_B8G8R8A8_TYPELESS;

	case DXGI_FORMAT_B8G8R8X8_UNORM:
	case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
		return DXGI_FORMAT_B8G8R8X8_TYPELESS;

		// 32-bit Z w/ Stencil
	case DXGI_FORMAT_R32G8X24_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
	case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
	case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
		return DXGI_FORMAT_R32G8X24_TYPELESS;

		// No Stencil
	case DXGI_FORMAT_R32_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT:
	case DXGI_FORMAT_R32_FLOAT:
		return DXGI_FORMAT_R32_TYPELESS;

		// 24-bit Z
	case DXGI_FORMAT_R24G8_TYPELESS:
	case DXGI_FORMAT_D24_UNORM_S8_UINT:
	case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
	case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
		return DXGI_FORMAT_R24G8_TYPELESS;

		// 16-bit Z w/o Stencil
	case DXGI_FORMAT_R16_TYPELESS:
	case DXGI_FORMAT_D16_UNORM:
	case DXGI_FORMAT_R16_UNORM:
		return DXGI_FORMAT_R16_TYPELESS;

	default:
		return defaultFormat;
	}
}

DXGI_FORMAT GpuTexture::GetUAVFormat(DXGI_FORMAT defaultFormat)
{
	switch (defaultFormat)
	{
	case DXGI_FORMAT_R8G8B8A8_TYPELESS:
	case DXGI_FORMAT_R8G8B8A8_UNORM:
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
		return DXGI_FORMAT_R8G8B8A8_UNORM;

	case DXGI_FORMAT_B8G8R8A8_TYPELESS:
	case DXGI_FORMAT_B8G8R8A8_UNORM:
	case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
		return DXGI_FORMAT_B8G8R8A8_UNORM;

	case DXGI_FORMAT_B8G8R8X8_TYPELESS:
	case DXGI_FORMAT_B8G8R8X8_UNORM:
	case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
		return DXGI_FORMAT_B8G8R8X8_UNORM;

	case DXGI_FORMAT_R32_TYPELESS:
	case DXGI_FORMAT_R32_FLOAT:
		return DXGI_FORMAT_R32_FLOAT;

#ifdef _DEBUG
	case DXGI_FORMAT_R32G8X24_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
	case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
	case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
	case DXGI_FORMAT_D32_FLOAT:
	case DXGI_FORMAT_R24G8_TYPELESS:
	case DXGI_FORMAT_D24_UNORM_S8_UINT:
	case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
	case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
	case DXGI_FORMAT_D16_UNORM:

		ray_assert(false, "Requested a UAV format for a depth stencil format.");
#endif

	default:
		return defaultFormat;
	}
}

DXGI_FORMAT GpuTexture::GetDSVFormat(DXGI_FORMAT defaultFormat)
{
	switch (defaultFormat)
	{
		// 32-bit Z w/ Stencil
	case DXGI_FORMAT_R32G8X24_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
	case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
	case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
		return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;

		// No Stencil
	case DXGI_FORMAT_R32_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT:
	case DXGI_FORMAT_R32_FLOAT:
		return DXGI_FORMAT_D32_FLOAT;

		// 24-bit Z
	case DXGI_FORMAT_R24G8_TYPELESS:
	case DXGI_FORMAT_D24_UNORM_S8_UINT:
	case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
	case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
		return DXGI_FORMAT_D24_UNORM_S8_UINT;

		// 16-bit Z w/o Stencil
	case DXGI_FORMAT_R16_TYPELESS:
	case DXGI_FORMAT_D16_UNORM:
	case DXGI_FORMAT_R16_UNORM:
		return DXGI_FORMAT_D16_UNORM;

	default:
		return defaultFormat;
	}
}

DXGI_FORMAT GpuTexture::GetDepthFormat(DXGI_FORMAT defaultFormat)
{
	switch (defaultFormat)
	{
		// 32-bit Z w/ Stencil
	case DXGI_FORMAT_R32G8X24_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
	case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
	case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
		return DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;

		// No Stencil
	case DXGI_FORMAT_R32_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT:
	case DXGI_FORMAT_R32_FLOAT:
		return DXGI_FORMAT_R32_FLOAT;

		// 24-bit Z
	case DXGI_FORMAT_R24G8_TYPELESS:
	case DXGI_FORMAT_D24_UNORM_S8_UINT:
	case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
	case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
		return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;

		// 16-bit Z w/o Stencil
	case DXGI_FORMAT_R16_TYPELESS:
	case DXGI_FORMAT_D16_UNORM:
	case DXGI_FORMAT_R16_UNORM:
		return DXGI_FORMAT_R16_UNORM;

	default:
		return DXGI_FORMAT_UNKNOWN;
	}
}

DXGI_FORMAT GpuTexture::GetStencilFormat(DXGI_FORMAT defaultFormat)
{
	switch (defaultFormat)
	{
		// 32-bit Z w/ Stencil
	case DXGI_FORMAT_R32G8X24_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
	case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
	case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
		return DXGI_FORMAT_X32_TYPELESS_G8X24_UINT;

		// 24-bit Z
	case DXGI_FORMAT_R24G8_TYPELESS:
	case DXGI_FORMAT_D24_UNORM_S8_UINT:
	case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
	case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
		return DXGI_FORMAT_X24_TYPELESS_G8_UINT;

	default:
		return DXGI_FORMAT_UNKNOWN;
	}
}

//--------------------------------------------------------------------------------------
// Return the BPP for a particular format
//--------------------------------------------------------------------------------------
size_t GpuTexture::BytesPerPixel(DXGI_FORMAT Format)
{
	switch (Format)
	{
	case DXGI_FORMAT_R32G32B32A32_TYPELESS:
	case DXGI_FORMAT_R32G32B32A32_FLOAT:
	case DXGI_FORMAT_R32G32B32A32_UINT:
	case DXGI_FORMAT_R32G32B32A32_SINT:
		return 16;

	case DXGI_FORMAT_R32G32B32_TYPELESS:
	case DXGI_FORMAT_R32G32B32_FLOAT:
	case DXGI_FORMAT_R32G32B32_UINT:
	case DXGI_FORMAT_R32G32B32_SINT:
		return 12;

	case DXGI_FORMAT_R16G16B16A16_TYPELESS:
	case DXGI_FORMAT_R16G16B16A16_FLOAT:
	case DXGI_FORMAT_R16G16B16A16_UNORM:
	case DXGI_FORMAT_R16G16B16A16_UINT:
	case DXGI_FORMAT_R16G16B16A16_SNORM:
	case DXGI_FORMAT_R16G16B16A16_SINT:
	case DXGI_FORMAT_R32G32_TYPELESS:
	case DXGI_FORMAT_R32G32_FLOAT:
	case DXGI_FORMAT_R32G32_UINT:
	case DXGI_FORMAT_R32G32_SINT:
	case DXGI_FORMAT_R32G8X24_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
	case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
	case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
		return 8;

	case DXGI_FORMAT_R10G10B10A2_TYPELESS:
	case DXGI_FORMAT_R10G10B10A2_UNORM:
	case DXGI_FORMAT_R10G10B10A2_UINT:
	case DXGI_FORMAT_R11G11B10_FLOAT:
	case DXGI_FORMAT_R8G8B8A8_TYPELESS:
	case DXGI_FORMAT_R8G8B8A8_UNORM:
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
	case DXGI_FORMAT_R8G8B8A8_UINT:
	case DXGI_FORMAT_R8G8B8A8_SNORM:
	case DXGI_FORMAT_R8G8B8A8_SINT:
	case DXGI_FORMAT_R16G16_TYPELESS:
	case DXGI_FORMAT_R16G16_FLOAT:
	case DXGI_FORMAT_R16G16_UNORM:
	case DXGI_FORMAT_R16G16_UINT:
	case DXGI_FORMAT_R16G16_SNORM:
	case DXGI_FORMAT_R16G16_SINT:
	case DXGI_FORMAT_R32_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT:
	case DXGI_FORMAT_R32_FLOAT:
	case DXGI_FORMAT_R32_UINT:
	case DXGI_FORMAT_R32_SINT:
	case DXGI_FORMAT_R24G8_TYPELESS:
	case DXGI_FORMAT_D24_UNORM_S8_UINT:
	case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
	case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
	case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
	case DXGI_FORMAT_R8G8_B8G8_UNORM:
	case DXGI_FORMAT_G8R8_G8B8_UNORM:
	case DXGI_FORMAT_B8G8R8A8_UNORM:
	case DXGI_FORMAT_B8G8R8X8_UNORM:
	case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
	case DXGI_FORMAT_B8G8R8A8_TYPELESS:
	case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
	case DXGI_FORMAT_B8G8R8X8_TYPELESS:
	case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
		return 4;

	case DXGI_FORMAT_R8G8_TYPELESS:
	case DXGI_FORMAT_R8G8_UNORM:
	case DXGI_FORMAT_R8G8_UINT:
	case DXGI_FORMAT_R8G8_SNORM:
	case DXGI_FORMAT_R8G8_SINT:
	case DXGI_FORMAT_R16_TYPELESS:
	case DXGI_FORMAT_R16_FLOAT:
	case DXGI_FORMAT_D16_UNORM:
	case DXGI_FORMAT_R16_UNORM:
	case DXGI_FORMAT_R16_UINT:
	case DXGI_FORMAT_R16_SNORM:
	case DXGI_FORMAT_R16_SINT:
	case DXGI_FORMAT_B5G6R5_UNORM:
	case DXGI_FORMAT_B5G5R5A1_UNORM:
	case DXGI_FORMAT_A8P8:
	case DXGI_FORMAT_B4G4R4A4_UNORM:
		return 2;

	case DXGI_FORMAT_R8_TYPELESS:
	case DXGI_FORMAT_R8_UNORM:
	case DXGI_FORMAT_R8_UINT:
	case DXGI_FORMAT_R8_SNORM:
	case DXGI_FORMAT_R8_SINT:
	case DXGI_FORMAT_A8_UNORM:
	case DXGI_FORMAT_P8:
		return 1;

	default:
		return 0;
	}
}

NODISCARD GpuResource&& GpuTextureAllocator::Allocate(GpuResourceDescription& textureDesc) noexcept
{
	D3D12_RESOURCE_DESC resourceDesc = {};
	resourceDesc.Width = textureDesc.Width;
	resourceDesc.Height = textureDesc.Height;
	resourceDesc.MipLevels = textureDesc.MipLevels;
	resourceDesc.DepthOrArraySize = textureDesc.ArraySize;
	resourceDesc.Dimension = textureDesc.Dimension;
	resourceDesc.Flags = textureDesc.Flags;
	resourceDesc.Format = textureDesc.Format;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	resourceDesc.SampleDesc = textureDesc.SampleDesc;

	auto resourceAllocationInfo = gDevice->GetResourceAllocationInfo(1, 1, &resourceDesc);
	resourceDesc.Alignment = resourceAllocationInfo.Alignment;
	
	if (!_currentPool->IsEnough(resourceAllocationInfo.SizeInBytes))
		_currentPool = &_memoryManager.RequestPool(resourceAllocationInfo.SizeInBytes);

	ID3D12Resource* resource;
	auto hr = gDevice->CreatePlacedResource(_currentPool->_heap, _currentPool->_offset, &resourceDesc, D3D12_RESOURCE_STATE_COPY_DEST, textureDesc.ClearValue, IID_PPV_ARGS(&resource));

	check(hr == S_OK)

		_currentPool->_offset += resourceAllocationInfo.SizeInBytes;
	_currentPool->_availableSize -= _currentPool->_offset;

	GpuTexture ret;
	ret._desc = std::move(textureDesc);
	ret._underlyingPool = _currentPool;
	ret._resource = resource;
	ret._resourceSize = resourceAllocationInfo.SizeInBytes;
	ret._usageState = D3D12_RESOURCE_STATE_COPY_DEST;

	return std::move(ret);
}

void GpuTextureAllocator::Free(GpuResource& resource) noexcept
{
	dynamic_cast<GpuTexture&>(resource)._resource->Release();
	if (resource.IsManaged())
	{
		dynamic_cast<GpuTextureMemoryPool*>(dynamic_cast<GpuTexture&>(resource)._underlyingPool)->_availableSize += dynamic_cast<GpuTexture&>(resource)._resourceSize;
	}

	// TODO: MemorySegment
}

bool GpuTexture::Create(GpuTextureDescription& textureDesc, pcstr debugName) noexcept
{
	ray_assert(textureDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE1D ||
		textureDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D ||
		textureDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE3D,
		"Resource dimension must be texture!")

		* dynamic_cast<GpuResource*>(this) = std::move(gTextureAllocator.Allocate(textureDesc));
	
#if defined(RAY_DEBUG) || defined(RAY_DEVELOPMENT)
	size_t debugNameSize = strlen(debugName);
	WCHAR dest[128];
	MultiByteToWideChar(0, 0, debugName, debugNameSize, dest, debugNameSize);
	dest[debugNameSize] = '\0';
	_resource->SetName(dest);
#endif

	if (textureDesc.UploadBufferData)
	{
		return Load(*gUploadBuffer, textureDesc.UploadBufferData);
	}

	return true;
}

bool GpuTexture::Load(UploadBuffer& uploadBuffer, const void* uploadBufferData) noexcept
{
	check(uploadBufferData != nullptr)

	CommandContext::InitializeTexture(*this, uploadBuffer);
	return true;
}

void GpuTexture::Release() noexcept
{
	gTextureAllocator.Free(*this);
}

void TextureView::Create(GpuResource& resource, DescriptorHeap* cbvSrvUavHeap, DescriptorHeap* rtvHeap, DescriptorHeap* dsvHeap) noexcept
{
	auto desc = resource.GetDesc();
	auto nativeResource = resource.GetNativeResource();

	_rtvHeap = rtvHeap != nullptr ? rtvHeap : &gDescriptorHeapsManager.GetCurrentRTV_Heap(true);
	_dsvHeap = dsvHeap != nullptr ? dsvHeap : &gDescriptorHeapsManager.GetCurrentDSV_Heap(true);
	_cbvSrvUavHeap = cbvSrvUavHeap != nullptr ? cbvSrvUavHeap : &gDescriptorHeapsManager.GetCurrentCBV_SRV_UAV_Heap(true);

	// ========================== RENDER TARGET VIEW ========================== //
	if (desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET)
	{
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.Format = desc.Format;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice = 0;
		rtvDesc.Texture2D.PlaneSlice = 0;

		_rtvHandle = _rtvHeap->Allocate(1);
		gDevice->CreateRenderTargetView(nativeResource, &rtvDesc, _rtvHandle.GetCpuHandle());
	}

	// ========================== DEPTH STENCIL VIEW ========================== //
	if (desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL)
	{
		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		dsvDesc.Format = GpuTexture::GetDSVFormat(desc.Format);
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice = 0;

		_dsvHandle[0] = _dsvHeap->Allocate(1);
		_dsvHandle[1] = _dsvHeap->Allocate(1);

		dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
		gDevice->CreateDepthStencilView(nativeResource, &dsvDesc, _dsvHandle[0].GetCpuHandle());

		dsvDesc.Flags = D3D12_DSV_FLAG_READ_ONLY_DEPTH;
		gDevice->CreateDepthStencilView(nativeResource, &dsvDesc, _dsvHandle[1].GetCpuHandle());
	}

	// ========================== SHADER RESOURCE VIEW ========================== //
	if ((desc.Flags & D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE) == 0)
	{
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = desc.Format;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // for now we are supporting only texture2d
		srvDesc.Texture2D.MipLevels = desc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.PlaneSlice = 0;
		srvDesc.Texture2D.ResourceMinLODClamp = 0.f;

		_srvHandle = _cbvSrvUavHeap->Allocate(1);
		gDevice->CreateShaderResourceView(nativeResource, &srvDesc, _srvHandle.GetCpuHandle());
	}

	// ========================== UNORDERED ACCESS VIEW ========================== //
	if (desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS)
	{
		D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
		uavDesc.Format = GpuTexture::GetUAVFormat(desc.Format);
		uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D; // for now we are supporting only texture2d
		uavDesc.Texture2D.PlaneSlice = 0;
		uavDesc.Texture2D.MipSlice = 0;

		_uavHandle = _cbvSrvUavHeap->Allocate(1);
		gDevice->CreateUnorderedAccessView(nativeResource, nullptr, &uavDesc, _uavHandle.GetCpuHandle());
	}
}

