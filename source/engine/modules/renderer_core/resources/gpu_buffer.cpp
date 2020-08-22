#include "gpu_buffer.hpp"
#include "../command_context.hpp"
#include "../renderer.hpp"
#include <core/debug/assert.hpp>
#include <core/math/common.hpp>

namespace math = ray::core::math;

namespace ray::renderer_core_api::resources
{
    void GpuBuffer::Create(u32 numElements, u32 elementSize, const void* initialData)
    {
        _bufferSize = numElements * elementSize;
        _elementCount = numElements;
        _elementSize = elementSize;

        auto desc = DescribeBuffer();

        _usageState = D3D12_RESOURCE_STATE_COMMON;

        D3D12_HEAP_PROPERTIES heapProps;
        heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        heapProps.CreationNodeMask = 1;
        heapProps.VisibleNodeMask = 1;
        heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        heapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
        
        check(globals::gDevice->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &desc, _usageState, nullptr, IID_PPV_ARGS(&_resource)) == S_OK);

        if (initialData)
            CommandContext::InitializeBuffer(*this, initialData, _bufferSize);

        _gpuVirtualAddress = _resource->GetGPUVirtualAddress();

        // CreateDerivedViews();
    }

    void GpuBuffer::CreatePlaces(ID3D12Heap* backingHeap, size_t heapOffset, u32 numElements, u32 elementSize, const void* initialData)
    {
        _bufferSize = numElements * elementSize;
        _elementCount = numElements;
        _elementSize = elementSize;

        auto desc = DescribeBuffer();

        _usageState = D3D12_RESOURCE_STATE_COMMON;

        check(globals::gDevice->CreatePlacedResource(backingHeap, heapOffset, &desc, _usageState, nullptr, IID_PPV_ARGS(&_resource)) == S_OK)

        if (initialData)
            CommandContext::InitializeBuffer(*this, initialData, _bufferSize);

        _gpuVirtualAddress = _resource->GetGPUVirtualAddress();

        // CreateDerivedViews();
    }

    D3D12_CPU_DESCRIPTOR_HANDLE GpuBuffer::CreateConstantBufferView(size_t offset, size_t size)
    {
        check(size + offset <= _bufferSize)

        size = math::AlignUp(size, 16);

        D3D12_CONSTANT_BUFFER_VIEW_DESC desc;
        desc.BufferLocation = _gpuVirtualAddress + offset;
        desc.SizeInBytes = size;

        auto cbvHandle = AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
        globals::gDevice->CreateConstantBufferView(&desc, cbvHandle);

        return cbvHandle;
    }

    D3D12_VERTEX_BUFFER_VIEW GpuBuffer::VertexBufferView(size_t offset, size_t size, u32 stride)
    {
        D3D12_VERTEX_BUFFER_VIEW vbView;
        vbView.BufferLocation = _gpuVirtualAddress + offset;
        vbView.SizeInBytes = size;
        vbView.StrideInBytes = stride;
        return vbView;
    }

    D3D12_INDEX_BUFFER_VIEW GpuBuffer::IndexBufferView(size_t offset, size_t size, bool b32Bit)
    {
        D3D12_INDEX_BUFFER_VIEW ibView;
        ibView.BufferLocation = _gpuVirtualAddress + offset;
        ibView.Format = b32Bit ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT;
        ibView.SizeInBytes = size;
        return ibView;
    }
    
    D3D12_RESOURCE_DESC GpuBuffer::DescribeBuffer() const noexcept
    {
        check(_bufferSize != 0)

        D3D12_RESOURCE_DESC desc;
        desc.Alignment = 0;
        desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        desc.Format = DXGI_FORMAT_UNKNOWN;
        desc.DepthOrArraySize = 1;
        desc.Flags = _resourceFlags;
        desc.Height = 1;
        desc.MipLevels = 1;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Width = static_cast<u64>(_bufferSize);
        return desc;
    }
}

