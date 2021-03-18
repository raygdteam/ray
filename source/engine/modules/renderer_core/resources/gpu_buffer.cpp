#include "gpu_buffer.hpp"
#include <renderer_core/command_context.hpp>
#include <renderer_core/renderer.hpp>
#include <renderer_core/descriptor_heap.hpp>
#include "upload_buffer.hpp"
#include <core/debug/assert.hpp>
#include <core/math/common.hpp>

void GpuBuffer::Create(GpuBufferDescription& desc, pcstr debugName) noexcept
{
	GpuResource::Create(gBufferAllocator, desc, debugName);

	if (desc.UploadBufferData)
	{
		Load(*gUploadBuffer, desc.UploadBufferData);
	}
}

bool GpuBuffer::Load(UploadBuffer& uploadBuffer, const void* uploadBufferData) noexcept
{
	check(uploadBufferData != nullptr)

	_desc.UploadBufferData = uploadBufferData;
	CommandContext::InitializeBuffer(*this, uploadBuffer);

	return true;
}

void GpuBuffer::Release() noexcept
{
	gBufferAllocator.Free(*this);
}


void BufferView::Create(GpuResource& resource, DescriptorHeap* cbvSrvUavHeap, DescriptorHeap* rtvHeap, DescriptorHeap* dsvHeap) noexcept
{
	auto desc = resource.GetDesc();
	auto nativeResource = resource.GetNativeResource();
	auto gpuVirtualAddress = nativeResource->GetGPUVirtualAddress();
	u32 numElements = desc.SizeInBytes / desc.Stride;

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
	if ((desc.Flags & D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE) == 0)
	{
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = desc.Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Buffer.FirstElement = 0;
		srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
		srvDesc.Buffer.NumElements = numElements;
		
		if (desc.Type == GpuBufferType::eStructured)
		{
			srvDesc.Buffer.StructureByteStride = desc.Stride;
		}
		else
		{
			srvDesc.Buffer.StructureByteStride = 0;
		}

		if (cbvSrvUavHeap == nullptr)
		{
			_srvHandle = AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		}
		else
		{
			_srvHandle = cbvSrvUavHeap->Allocate(1);
		}
		gDevice->CreateShaderResourceView(nativeResource, &srvDesc, _srvHandle.GetCpuHandle());
	}

	// ========================== UNORDERED ACCESS VIEW ========================== //
	if (desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS)
	{
		D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
		uavDesc.Format = GpuTexture::GetUAVFormat(desc.Format);
		uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
		uavDesc.Buffer.CounterOffsetInBytes = 0;
		uavDesc.Buffer.FirstElement = 0;
		uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
		uavDesc.Buffer.NumElements = numElements;
		uavDesc.Buffer.StructureByteStride = desc.Stride;

		if (cbvSrvUavHeap == nullptr)
		{
			_uavHandle = AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		}
		else
		{
			_uavHandle = cbvSrvUavHeap->Allocate(1);
		}
		gDevice->CreateUnorderedAccessView(nativeResource, nullptr, &uavDesc, _uavHandle.GetCpuHandle());
		gDevice->CreateUnorderedAccessView(nativeResource, nullptr, &uavDesc, _uavHandle.GetCpuHandle());
	}
}


