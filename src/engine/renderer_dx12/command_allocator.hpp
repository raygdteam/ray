#pragma once
#include <renderer_core/command_allocator.hpp>
#include <d3d12.h>

using namespace ray::renderer_core_api;

namespace ray::renderer::d3d12
{
	class D3D12CommandAllocator : public ICommandAllocator
	{
	public:
		D3D12CommandAllocator() {}

		bool Reset() override;
	};
}