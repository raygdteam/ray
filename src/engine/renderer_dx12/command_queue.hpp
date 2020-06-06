#pragma once
#include <renderer_core/command_queue.hpp>

using namespace ray::renderer_core_api;

namespace ray::renderer::d3d12
{
	class D3D12CommandQueue : public ICommandQueue
	{
	public:
		D3D12CommandQueue() {}

	};
}