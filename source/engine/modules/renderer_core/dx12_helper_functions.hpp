#pragma once
#include <core/core.hpp>
#include <d3d12.h>

struct GpuResourceDescription;

D3D12_RESOURCE_DESC DescribeBuffer(u64 size) noexcept;
D3D12_RESOURCE_DESC DescribeDefaultTexture2D(DXGI_FORMAT format, u64 width, u64 height) noexcept;
D3D12_RESOURCE_DESC DescribeMsaaTexture2D() noexcept;
D3D12_HEAP_PROPERTIES DescribeHeapProps(D3D12_HEAP_TYPE heapType) noexcept;
D3D12_HEAP_DESC DescribeHeap(D3D12_HEAP_PROPERTIES& heapProps, D3D12_HEAP_FLAGS flags, u64 size) noexcept;
D3D12_HEAP_DESC DescribeMsaaHeap(D3D12_HEAP_PROPERTIES& heapProps, D3D12_HEAP_FLAGS flags, u64 size) noexcept;
D3D12_RESOURCE_DESC ConvertResourceDescToD3D12_Type(GpuResourceDescription& desc) noexcept;
D3D12_TEXTURE_LAYOUT GetLayout(D3D12_RESOURCE_DIMENSION dimension) noexcept;
