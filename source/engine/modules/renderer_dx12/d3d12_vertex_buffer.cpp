#include "pch.hpp"
#include "d3d12_vertex_buffer.hpp"

namespace ray::renderer::d3d12::resources
{
	D3D12VertexBuffer::~D3D12VertexBuffer()
	{
		if (GetInstance())
			static_cast<ID3D12Resource*>(GetInstance())->Release();
	}

	bool D3D12VertexBuffer::Map(u32 subresourceIndex, u32 start, u32 end, void* data)
	{
		auto temp = static_cast<ID3D12Resource*>(GetInstance());
		CD3DX12_RANGE range(start, end);
		auto hResult = temp->Map(subresourceIndex, &range, static_cast<void**>(data));
		return hResult == S_OK;
	}

	void D3D12VertexBuffer::Unmap(u32 subresourceIndex, u32 start, u32 end)
	{
		auto temp = static_cast<ID3D12Resource*>(GetInstance());
		CD3DX12_RANGE range(start, end);
		temp->Unmap(subresourceIndex, &range);
	}
}
