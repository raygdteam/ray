#pragma once
#include <renderer_core/descriptor_heap.hpp>

using namespace ray::renderer_core_api;

namespace ray::renderer::d3d12
{
	class D3D12DescriptorHeap : public IDescriptorHeap
	{
	public:
		D3D12DescriptorHeap() {}
		~D3D12DescriptorHeap();
	};

	class D3D12CPUDescriptor : public ICPUDescriptor
	{
	public:
		D3D12CPUDescriptor() {}
		bool Initialize(IDescriptorHeap* descriptorHeap) override;
		bool Offset(u32 step) override;
	};

	class D3D12GPUDescriptor : public IGPUDescriptor
	{
	public:
		D3D12GPUDescriptor() {}
		bool Initialize(IDescriptorHeap* descriptorHeap) override { return true; }
		bool Offset(u32 step) override {}
	};
}