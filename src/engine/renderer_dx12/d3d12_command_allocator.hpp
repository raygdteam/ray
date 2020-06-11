#pragma once

namespace ray::renderer::d3d12
{
	class D3D12CommandAllocator : public ICommandAllocator
	{
	public:
		D3D12CommandAllocator() {}
		~D3D12CommandAllocator();

		bool Reset() override;
	};
}