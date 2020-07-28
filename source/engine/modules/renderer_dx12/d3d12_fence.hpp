#pragma once

namespace ray::renderer::d3d12
{
	class D3D12Fence : public IFence
	{
	public:
		D3D12Fence() {}
		~D3D12Fence();

		bool SetEventOnCompletion(IFenceEvent* fenceEvent, u64 value) override;
		u64 GetCompletedValue() override;
		bool Signal(u64 fenceValue) override;
	};

	class D3D12FenceEvent : public IFenceEvent
	{
	public:
		D3D12FenceEvent() {}
		~D3D12FenceEvent();

		u32 WaitFor() override;
		u32 WaitFor(u32 ms) override;
	};
}