#include <renderer_core/command_allocator.hpp>

using namespace ray::renderer_core_api;

namespace ray::renderer::d3d12
{
	class D3D12CommandAllocator : public ICommandAllocator
	{
	public:
		D3D12CommandAllocator() {}
	};
}

ICommandAllocator* GetRendererCommandAllocator()
{
	return new ray::renderer::d3d12::D3D12CommandAllocator;
}