#include "pch.hpp"
#include "d3d12_resources.hpp"
#include "utils.hpp"

namespace ray::renderer::d3d12::resources
{
	void D3D12ResourceBarrier::Transition(IResource* inResource, ResourceState beforeState, ResourceState afterState)
	{
		auto before = utils::ConvertResourceStateToD3D12(beforeState);
		auto after = utils::ConvertResourceStateToD3D12(afterState);
		
		auto tempResource = static_cast<ID3D12Resource*>(inResource->GetInstance());
		if (GetInstance())
			delete GetInstance();
			   
		auto resourceBarrier = new CD3DX12_RESOURCE_BARRIER(CD3DX12_RESOURCE_BARRIER::Transition(tempResource, before, after));
		SetInstance(resourceBarrier);
	}
	
	D3D12ResourceBarrier::~D3D12ResourceBarrier()
	{
		if (GetInstance())
		{
			auto instance = static_cast<CD3DX12_RESOURCE_BARRIER*>(GetInstance());
			delete instance;
		}
	}

	D3D12Resource::~D3D12Resource()
	{
		if (GetInstance())
			static_cast<ID3D12Resource*>(GetInstance())->Release();
		
	}

	bool D3D12Resource::Map(u32 subresourceIndex, Range* range, void** data)
	{
		auto temp = static_cast<ID3D12Resource*>(GetInstance());
		if (range == nullptr)
			return temp->Map(subresourceIndex, nullptr, data) == S_OK;

		CD3DX12_RANGE d3dRange(range->Start, range->End);
		return temp->Map(subresourceIndex, &d3dRange, data) == S_OK;
	}

	void D3D12Resource::Unmap(u32 subresourceIndex, Range* range)
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

	GpuVirtualAddress D3D12Resource::GetGpuVirtualAddress()
	{
		auto temp = static_cast<ID3D12Resource*>(GetInstance());
		return static_cast<GpuVirtualAddress>(temp->GetGPUVirtualAddress());
	}

}