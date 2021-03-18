#include "dx12_helper_functions.hpp"
#include <core/math/common.hpp>
#include <core/debug/assert.hpp>

#include "resources/gpu_texture.hpp"

D3D12_RESOURCE_DESC DescribeBuffer(u64 size) noexcept
{
	D3D12_RESOURCE_DESC desc = {};
	desc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.DepthOrArraySize = 1;
	desc.Height = 1;
	desc.MipLevels = 1;
	desc.Flags = D3D12_RESOURCE_FLAG_NONE;
	desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Width = size;
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;

	return desc;
}

D3D12_RESOURCE_DESC DescribeDefaultTexture2D(DXGI_FORMAT format, u64 width, u64 height) noexcept
{
	D3D12_RESOURCE_DESC desc = {};
	desc.MipLevels = 1;
	desc.DepthOrArraySize = 1;
	desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Flags = D3D12_RESOURCE_FLAG_NONE;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.Format = format;
	desc.Width = width;
	desc.Height = height;
	if (width * height * GpuTexture::BytesPerPixel(format) <= KB(64))
		desc.Alignment = D3D12_SMALL_RESOURCE_PLACEMENT_ALIGNMENT;
	else
		desc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;

	return desc;
}

D3D12_RESOURCE_DESC DescribeMsaaTexture2D() noexcept
{
	return D3D12_RESOURCE_DESC();
}

D3D12_HEAP_PROPERTIES DescribeHeapProps(D3D12_HEAP_TYPE heapType) noexcept
{
	D3D12_HEAP_PROPERTIES heapProps = {};
	heapProps.CreationNodeMask = 1;
	heapProps.VisibleNodeMask = 1;
	heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProps.Type = heapType;

	return heapProps;
}

D3D12_HEAP_DESC DescribeHeap(D3D12_HEAP_PROPERTIES& heapProps, D3D12_HEAP_FLAGS flags, u64 size) noexcept
{
	check(IsAligned(size, KB(64)))

		D3D12_HEAP_DESC desc = {};
	desc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
	desc.Flags = flags;
	desc.Properties = heapProps;
	desc.SizeInBytes = size;

	return desc;
}

D3D12_HEAP_DESC DescribeMsaaHeap(D3D12_HEAP_PROPERTIES& heapProps, D3D12_HEAP_FLAGS flags, u64 size) noexcept
{
	check(IsAligned(size, MB(4)))

		D3D12_HEAP_DESC desc = {};
	desc.Alignment = D3D12_DEFAULT_MSAA_RESOURCE_PLACEMENT_ALIGNMENT;
	desc.Flags = flags;
	desc.Properties = heapProps;
	desc.SizeInBytes = size;

	return desc;
}

D3D12_RESOURCE_DESC ConvertResourceDescToD3D12_Type(GpuResourceDescription& desc) noexcept
{
	D3D12_RESOURCE_DESC ret;
	ret.Dimension = desc.Dimension;
	ret.Format = desc.Format;
	ret.MipLevels = desc.MipLevels;
	ret.DepthOrArraySize = desc.ArraySize;
	ret.Flags = desc.Flags;
	ret.Format = desc.Format;
	ret.Height = desc.Height;
	ret.Layout = GetLayout(desc.Dimension);
	ret.SampleDesc = desc.SampleDesc;
	ret.Width = desc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER ? desc.SizeInBytes : desc.Width;

	return ret;
}

D3D12_TEXTURE_LAYOUT GetLayout(D3D12_RESOURCE_DIMENSION dimension) noexcept
{
	if (dimension == D3D12_RESOURCE_DIMENSION_BUFFER)
		return D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	return D3D12_TEXTURE_LAYOUT_UNKNOWN;
}
