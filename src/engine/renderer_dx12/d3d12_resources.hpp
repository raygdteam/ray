#pragma once

namespace ray::renderer::d3d12::resources
{
	class D3D12Resource : public IResource
	{
	public:
		~D3D12Resource();
	};

	class D3D12ResourceBarrier : public IResourceBarrier
	{
	public:
		

		void Transition(IResource* inResource, ResourceState beforeState, ResourceState afterState) override;
	};
}