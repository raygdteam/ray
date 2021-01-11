#include "sampler_manager.hpp"
#include "renderer.hpp"

namespace ray::renderer_core_api
{
	D3D12_CPU_DESCRIPTOR_HANDLE SamplerDesc::CreateDescriptor()
	{
		auto handle = AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
		CreateDescriptor(handle);
		return handle;
	}

	void SamplerDesc::CreateDescriptor(D3D12_CPU_DESCRIPTOR_HANDLE& handle)
	{
		globals::gDevice->CreateSampler(this, handle);
	}
}
