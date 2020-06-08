#pragma once
#include <renderer_core/command_list.hpp>

using namespace ray::renderer_core_api;

namespace ray::renderer::d3d12
{
	class D3D12CommandList : public ICommandList
	{
	public:
		D3D12CommandList() {}

		bool Reset(ICommandAllocator* commnandAllocator, IPipelineState* pipelineState) override;
		void Close() override;
	};
}
