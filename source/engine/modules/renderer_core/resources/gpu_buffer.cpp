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

}

