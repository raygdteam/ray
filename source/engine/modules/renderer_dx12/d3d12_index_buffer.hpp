#pragma once

namespace ray::renderer::d3d12::resources
{
    class D3D12IndexBuffer :
        public IIndexBuffer
    {
    public:
        ~D3D12IndexBuffer();

        bool Map(u32 subresourceIndex, Range* range, void** data) override;
        void Unmap(u32 subresourceIndex, Range* range) override;

        GpuVirtualAddress GetGpuVirtualAddress() override;
    };

}


