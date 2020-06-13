#include "pch.hpp"
#include "d3d12_resources.hpp"

namespace ray::renderer::d3d12::resources
{
	void D3D12ResourceBarrier::Transition(IResource* inResource, ResourceState beforeState, ResourceState afterState)
	{
		D3D12_RESOURCE_STATES before, after;
		switch (beforeState)
		{
		case ray::renderer_core_api::resources::ResourceState::present:
			before = D3D12_RESOURCE_STATE_PRESENT;
			break;
		case ray::renderer_core_api::resources::ResourceState::render_target:
			before = D3D12_RESOURCE_STATE_RENDER_TARGET;
			break;
		default:
			return;
			break;
		}
		switch (afterState)
		{
		case ray::renderer_core_api::resources::ResourceState::present:
			after = D3D12_RESOURCE_STATE_PRESENT;
			break;
		case ray::renderer_core_api::resources::ResourceState::render_target:
			after = D3D12_RESOURCE_STATE_RENDER_TARGET;
			break;
		default:
			return;
			break;
		}
		auto tempResource = static_cast<ID3D12Resource*>(inResource->GetInstance());
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

}