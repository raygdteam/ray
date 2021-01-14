#include "gpu_buffer.hpp"
#include "../command_context.hpp"
#include "../renderer.hpp"
#include "../descriptor_heap.hpp"
#include "upload_buffer.hpp"
#include <core/debug/assert.hpp>
#include <core/math/common.hpp>

namespace math = ray::core::math;

namespace ray::renderer_core_api::resources
{
    GpuResource&& GpuBufferAllocator::Allocate(GpuResourceDescription& bufferDesc) noexcept
    {
        D3D12_RESOURCE_DESC resourceDesc = {};
        resourceDesc.Width = bufferDesc.SizeInBytes;
        resourceDesc.Height = 1;
        resourceDesc.MipLevels = 1;
        resourceDesc.DepthOrArraySize = 1;
        resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        resourceDesc.Flags = bufferDesc.Flags;
        resourceDesc.Format = bufferDesc.Format;
        resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        resourceDesc.SampleDesc.Count = 1;
        resourceDesc.SampleDesc.Quality = 0;

        auto resourceAllocationInfo = globals::gDevice->GetResourceAllocationInfo(1, 1, &resourceDesc);
        resourceDesc.Alignment = resourceAllocationInfo.Alignment;

        if (_currentPool == nullptr || !_currentPool->IsEnough(resourceAllocationInfo.SizeInBytes))
            _currentPool = &_memoryManager.RequestPool(resourceAllocationInfo.SizeInBytes);

        ID3D12Resource* resource;
        auto hr = globals::gDevice->CreatePlacedResource(_currentPool->_heap, _currentPool->_offset, &resourceDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&resource));

        check(hr == S_OK)

        _currentPool->_offset += resourceAllocationInfo.SizeInBytes;
        _currentPool->_availableSize -= _currentPool->_offset;

        GpuBuffer ret;
        ret._desc = std::move(bufferDesc);
        ret._underlyingPool = _currentPool;
        ret._resource = resource;
        ret._resourceSize = resourceAllocationInfo.SizeInBytes;
        ret._usageState = D3D12_RESOURCE_STATE_COPY_DEST;

        return std::move(ret);
    }

    void GpuBufferAllocator::Free(GpuResource& resource) noexcept
    {
        dynamic_cast<GpuBuffer&>(resource)._resource->Release();
        dynamic_cast<GpuBuffer&>(resource)._underlyingPool->_availableSize += dynamic_cast<GpuBuffer&>(resource)._resourceSize;

        // TODO: MemorySegment
    }

    void GpuBuffer::Create(GpuBufferDescription& desc) noexcept
    {
        *dynamic_cast<GpuResource*>(this) = globals::gBufferAllocator.Allocate(desc);

        if(desc.UploadBufferData)
        {
            Load(desc.UploadBufferData);
        }
    }

    bool GpuBuffer::Load(const void* uploadBufferData) noexcept
    {
        check(uploadBufferData != nullptr)

        UploadBuffer ub;
        CommandContext::InitializeBuffer(*this, ub);

        return true;
    }

    void GpuBuffer::Release() noexcept
    {
        globals::gBufferAllocator.Free(*this);
    }


    void BufferView::Create(GpuResource& resource) noexcept
    {
        auto desc = resource.GetDesc();
        auto nativeResource = resource.GetNativeResource();
        auto gpuVirtualAddress = nativeResource->GetGPUVirtualAddress();
        u32 numElements = desc.SizeInBytes / desc.Stride;
        desc.Flags;

        // ========================== VERTEX BUFFER VIEW ========================== //
        _vbView.BufferLocation = gpuVirtualAddress;
        _vbView.SizeInBytes = desc.SizeInBytes;
        _vbView.StrideInBytes = desc.Stride;

        // ========================== INDEX BUFFER VIEW ========================== //
        _ibView.BufferLocation = gpuVirtualAddress;
        _ibView.Format = desc.Stride == 4 ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT;
        _ibView.SizeInBytes = desc.SizeInBytes;

        // ========================== CONSTANT BUFFER VIEW ========================== //
        _cbView = gpuVirtualAddress;

        // ========================== SHADER RESOURCE VIEW ========================== //
        if (desc.Flags & D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE == 0)
        {
            D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
            srvDesc.Format = desc.Format;
            srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
            srvDesc.Buffer.FirstElement = 0;
            srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
            srvDesc.Buffer.NumElements = numElements;
            srvDesc.Buffer.StructureByteStride = desc.Stride;

            // TODO:
            // auto& descriptorHeap = gDescriptorHeapsManager.GetCurrentSRV_Heap();
            // _srvHandle = descriptorHeap.Allocate(1);
            globals::gDevice->CreateShaderResourceView(nativeResource, &srvDesc, _srvHandle.GetCpuHandle());
        }
        
        // ========================== UNORDERED ACCESS VIEW ========================== //
        if (desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS)
        {
            D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc;
            uavDesc.Format = desc.Format;
            uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
            uavDesc.Buffer.CounterOffsetInBytes = 0;
            uavDesc.Buffer.FirstElement = 0;
            uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
            uavDesc.Buffer.NumElements = numElements;
            uavDesc.Buffer.StructureByteStride = desc.Stride;

            // TODO:
            // _uavHandle = descriptorHeap.Allocate(1);
            globals::gDevice->CreateUnorderedAccessView(nativeResource, nullptr, &uavDesc, _uavHandle.GetCpuHandle());
        }
    }
}

