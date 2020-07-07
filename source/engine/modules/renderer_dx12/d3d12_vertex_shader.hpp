#pragma once

namespace ray::renderer::d3d12
{
    class D3D12VertexShader :
        public IVertexShader
    {
    public:
        ~D3D12VertexShader();

        bool Compile(pcstr path) override;

    };

}


