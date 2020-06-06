#pragma once
#include <renderer_core/swap_chain.hpp>

#include <d3d12.h>
#include <dxgi.h>
#include <dxgi1_4.h>
#include <combaseapi.h>

using namespace ray::renderer_core_api;
using namespace ray::renderer_core_api::resources;

namespace ray::renderer::d3d12
{
	class D3D12SwapChain : public ISwapChain
	{
	public:
		D3D12SwapChain() {}

		bool Initialize(SwapChainDesc& desc, u32& frameIndex) override;
		bool GetBuffer(u32 index, IResource* resource) override;
	};
}