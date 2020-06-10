#pragma once
#include <renderer_core/resources/resources.hpp>

using namespace ray::renderer_core_api::resources;

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