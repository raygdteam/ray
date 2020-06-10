#pragma once
#include <renderer_core/command_list.hpp>

using namespace ray::renderer_core_api;
using namespace ray::renderer_core_api::resources;

namespace ray::renderer::d3d12
{
	class D3D12CommandList : public ICommandList
	{
	public:
		D3D12CommandList() {}
		~D3D12CommandList();

		bool Reset(ICommandAllocator* commnandAllocator, IPipelineState* pipelineState) override;
		void ResourceBarrier(IResourceBarrier* inResourceBarrier, u32 numBarriers) override;
		void OMSetRenderTargetView(u32 numRTs, ICPUDescriptor* inRtvHandle, ICPUDescriptor* inDsHandle, bool bRTsSingleHandleToDescriptorRange) override;
		void ClearRenderTarget(ICPUDescriptor* inRtHandle, float* rgba) override;
		bool Close() override;
	};
}
