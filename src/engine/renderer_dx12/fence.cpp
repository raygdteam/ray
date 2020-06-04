#include <renderer_core/fence.hpp>

using namespace ray::renderer_core_api;

namespace ray::renderer::d3d12
{
	class D3D12Fence : public IFence
	{
	public:
		D3D12Fence() {}
	};
}

IFence* GetRendererFence()
{
	return new ray::renderer::d3d12::D3D12Fence;
}