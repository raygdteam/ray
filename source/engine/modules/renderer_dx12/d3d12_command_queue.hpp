#pragma once

namespace ray::renderer::d3d12
{
	class D3D12CommandQueue : public ICommandQueue
	{
	public:
		D3D12CommandQueue() {}
		~D3D12CommandQueue();

		void SetCommandLists(ICommandList** commandLists, size_t numLists) override;
		void ExecuteCommandLists() override;
		bool Signal(IFence* fence, u32 fenceValue) override;
		bool Wait(IFence* fence, u64 fenceValue) override;

	private:
		ID3D12CommandList** _d3d12_lists;
		size_t _size;
	};
}