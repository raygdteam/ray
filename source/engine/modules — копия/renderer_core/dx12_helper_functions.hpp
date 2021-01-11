#pragma once
#include <core/core.hpp>
#include <d3d12.h>

namespace ray::dx12
{
	D3D12_RESOURCE_DESC DescribeBuffer(u64 size);
	D3D12_RESOURCE_DESC DescribeDefaultTexture2D(DXGI_FORMAT format, u64 width, u64 height);
	D3D12_RESOURCE_DESC DescribeMsaaTexture2D();
	D3D12_HEAP_PROPERTIES DescribeHeapProps(D3D12_HEAP_TYPE heapType);
	D3D12_HEAP_DESC DescribeHeap(D3D12_HEAP_PROPERTIES& heapProps, D3D12_HEAP_FLAGS flags, u64 size);
	D3D12_HEAP_DESC DescribeMsaaHeap(D3D12_HEAP_PROPERTIES& heapProps, D3D12_HEAP_FLAGS flags, u64 size);
}

