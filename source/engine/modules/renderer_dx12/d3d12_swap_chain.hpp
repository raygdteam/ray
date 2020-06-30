#pragma once

namespace ray::renderer::d3d12
{
	class D3D12SwapChain : public ISwapChain
	{
	public:
		D3D12SwapChain() {}
		~D3D12SwapChain();

		bool Initialize(SwapChainDesc& desc, u32& frameIndex) override;
		bool GetBuffer(u32 index, IResource* resource) override;
		u32 GetCurrentBackBufferIndex() override;
		bool Present(u32 syncInterval, u32 flags) override;
	};
}