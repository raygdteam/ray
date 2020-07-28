#include "pch.hpp"
#include "d3d12_index_buffer.hpp"

namespace ray::renderer::d3d12::resources
{
	D3D12IndexBuffer::~D3D12IndexBuffer()
	{
		if (GetInstance())
			static_cast<ID3D12Resource*>(GetInstance())->Release();
	}

	bool D3D12IndexBuffer::Map(u32 subresourceIndex, Range* range, void** data)
	{
		auto temp = static_cast<ID3D12Resource*>(GetInstance());
		if (range == nullptr)
			return temp->Map(subresourceIndex, nullptr, data) == S_OK;

		CD3DX12_RANGE d3dRange(range->Start, range->End);
		return temp->Map(subresourceIndex, &d3dRange, data) == S_OK;
	}

	void D3D12IndexBuffer::Unmap(u32 subresourceIndex, Range* range)
	{
		auto temp = static_cast<ID3D12Resource*>(GetInstance());
		if (range == nullptr)
		{
			temp->Unmap(subresourceIndex, nullptr);
			return;
		}
		CD3DX12_RANGE d3dRange(range->Start, range->End);
		temp->Unmap(subresourceIndex, &d3dRange);
	}

	GpuVirtualAddress D3D12IndexBuffer::GetGpuVirtualAddress()
	{
		auto temp = static_cast<ID3D12Resource*>(GetInstance());
		return static_cast<GpuVirtualAddress>(temp->GetGPUVirtualAddress());
	}
}
