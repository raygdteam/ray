#include <renderer_core/swap_chain.hpp>

using namespace ray::renderer_core_api;

namespace ray::renderer::d3d12
{
	class D3D12SwapChain : public ISwapChain
	{
	public:
		D3D12SwapChain() {}
	};
}

ISwapChain* GetRendererSwapChain()
{
	return new ray::renderer::d3d12::D3D12SwapChain;
}