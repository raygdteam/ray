#pragma once

namespace ray::renderer::d3d12
{
	class D3D12CommandList : public ICommandList
	{
	public:
		D3D12CommandList() {}
		~D3D12CommandList();

		bool Reset(ICommandAllocator* commnandAllocator, IPipelineState* pipelineState) override;
		//void ResourceBarrier(IResourceBarrier* inResourceBarrier, u32 numBarriers) override;
		void Transition(resources::ResourceTransitionStateDesc* desc, u32 descCount) override;
		void OMSetRenderTargetView(u32 numRTs, ICPUDescriptor* inRtvHandle, ICPUDescriptor* inDsHandle, bool bRTsSingleHandleToDescriptorRange) override;
		void SetPipelineState(IPipelineState* pso) override;
		void ClearRenderTarget(ICPUDescriptor* inRtHandle, float* rgba) override;
		u64 UpdateSubresource(IResource* desc, IResource* src, u64 offset, u64 indexSubresource, u64 subresourcesCount) override;
		bool Close() override;
	};
}
