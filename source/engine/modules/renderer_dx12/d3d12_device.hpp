#pragma once

//Direct3D 12 Device implementation

namespace ray::renderer::d3d12
{
    class D3D12Device : public IDevice
    {
    public:
        D3D12Device() {}
        ~D3D12Device();

        void SetName(pcstr debugName) override;

        bool Initialize() override;
        bool CreateCommandQueue(CommandQueueDesc& desc, ICommandQueue* outCommandQueue) override;
        bool CreateDescriptorHeap(DescriptorHeapDesc& desc, IDescriptorHeap* outDescriptorHeap) override;
        void CreateRenderTargetView(IResource* outResource, RenderTargetViewDesc& rtvDesc, ICPUDescriptor* descriptor) override;
        bool CreateCommandAllocator(ICommandAllocator* outCommandAllocator, CommandListType listType) override;
        bool CreateCommandList(ICommandList* outCommandList, ICommandAllocator* commandAllocator, IPipelineState* pipelineState, CommandListType listType) override;
        bool CreateFence(IFence* outFence, u64 fenceValue) override;
        bool CreateFenceEvent(IFenceEvent* outFenceEvent, pcstr name, bool bManualReset, bool bInitialState) override;
        bool CreateRootSignature(RootSignatureDesc& desc, IRootSignature* outRootSignature) override;
        bool CreatePipelineState(PipelineStateDesc& inDesc, IPipelineState* outPipelineState) override;
        bool CreateCommittedResource(ResourceDesc& inDesc, ResourceUsage usage, IResource* outResource) override;
        s32 GetDescriptorHandleIncrementSize(DescriptorHeapType type) override;

    private:


    };

}