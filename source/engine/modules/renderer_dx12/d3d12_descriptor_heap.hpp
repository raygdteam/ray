#pragma once

namespace ray::renderer::d3d12
{
	class D3D12DescriptorHeap : public IDescriptorHeap
	{
	public:
		D3D12DescriptorHeap() {}
		~D3D12DescriptorHeap();

		void GetCPUDescriptorHandleForHeapStart(ICPUDescriptor* outHandle) override;
		void GetGPUDescriptorHandleForHeapStart(IGPUDescriptor* outHandle) override;
	};

	class D3D12CPUDescriptor : public ICPUDescriptor
	{
	public:
		D3D12CPUDescriptor() {}
		~D3D12CPUDescriptor();

		bool Offset(u32 step) override;
		bool Increment() override;
	};

	class D3D12GPUDescriptor : public IGPUDescriptor
	{
	public:
		D3D12GPUDescriptor() {}

		bool Offset(u32 step) override { return true; }
		bool Increment() override { return true; }
	};
}