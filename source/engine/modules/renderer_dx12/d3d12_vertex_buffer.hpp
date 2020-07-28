#pragma once

namespace ray::renderer::d3d12::resources
{
    class D3D12VertexBuffer :
        public IVertexBuffer
    {
    public:
        ~D3D12VertexBuffer();

        bool Map(u32 subresourceIndex, Range* range, void** data) override;
        void Unmap(u32 subresourceIndex, Range* range) override;
    
        GpuVirtualAddress GetGpuVirtualAddress() override;
    };
}



