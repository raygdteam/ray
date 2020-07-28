#pragma once

namespace ray::renderer::d3d12::resources
{
	class D3D12Resource : public IResource
	{
	public:
		~D3D12Resource();

		bool Map(u32 subresourceIndex, Range* range, void** data) override;
		void Unmap(u32 subresourceIndex, Range* range) override;

		GpuVirtualAddress GetGpuVirtualAddress() override;
	};

	class D3D12ResourceBarrier : public IResourceBarrier
	{
	public:
		~D3D12ResourceBarrier();

		void Transition(IResource* inResource, ResourceState beforeState, ResourceState afterState) override;
	};
}