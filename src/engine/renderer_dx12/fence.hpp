#pragma once
#include <renderer_core/fence.hpp>

using namespace ray::renderer_core_api;

namespace ray::renderer::d3d12
{
	class D3D12Fence : public IFence
	{
	public:
		D3D12Fence() {}
	};

	class D3D12FenceEvent : public IFenceEvent
	{
	public:
		D3D12FenceEvent() {}
	};
}