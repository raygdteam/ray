#include "sampler_manager.hpp"
#include "renderer.hpp"

D3D12_CPU_DESCRIPTOR_HANDLE SamplerDesc::CreateDescriptor()
{
	auto heap = gDescriptorHeapsManager.GetCurrentSamplerHeap(true);
	auto handle = heap.Allocate(1);
	auto ret = handle.GetCpuHandle();
	CreateDescriptor(ret);
	return ret;
}

void SamplerDesc::CreateDescriptor(D3D12_CPU_DESCRIPTOR_HANDLE& handle)
{
	gDevice->CreateSampler(this, handle);
}
