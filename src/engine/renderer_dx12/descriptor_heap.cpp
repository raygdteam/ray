#include <renderer_core/descriptor_heap.hpp>

using namespace ray::renderer_core_api;

namespace ray::renderer::d3d12
{
	class D3D12DescriptorHeap : public IDescriptorHeap
	{
	public:
		D3D12DescriptorHeap() {}
	};
}

IDescriptorHeap* GetRendererDescriptorHeap()
{
	return new ray::renderer::d3d12::D3D12DescriptorHeap;
}