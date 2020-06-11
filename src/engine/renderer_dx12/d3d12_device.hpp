#pragma once

//Direct3D 12 Device implementation

namespace ray::renderer::d3d12
{
    class D3D12Device : public IDevice
    {
    public:
        D3D12Device() {}
        ~D3D12Device();

        bool Initialize() override;
        bool CreateCommandQueue(CommandQueueDesc& desc, ICommandQueue* cmdQueue) override;
        bool CreateDescriptorHeap(DescriptorHeapDesc& desc, IDescriptorHeap* descriptorHeap) override;
        void CreateRenderTargetView(IResource* resource, RenderTargetViewDesc& rtvDesc, ICPUDescriptor* descriptor) override;
        bool CreateCommandAllocator(ICommandAllocator* commandAllocator, CommandListType listType) override;
        bool CreateCommandList(ICommandList* commandList, ICommandAllocator* commandAllocator, IPipelineState* pipelineState, CommandListType listType) override;
        bool CreateFence(IFence* outFence, u64 fenceValue) override;
        bool CreateFenceEvent(IFenceEvent* outFenceEvent, pcstr name, bool bManualReset, bool bInitialState) override;
        s32 GetDescriptorHandleIncrementSize(DescriptorHeapType type) override;

    private:


    };

}