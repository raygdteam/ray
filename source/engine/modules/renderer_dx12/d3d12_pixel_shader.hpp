#pragma once

namespace ray::renderer::d3d12
{
    class D3D12PixelShader :
        public IPixelShader
    {
    public:
        ~D3D12PixelShader();

        bool Compile(pcstr) override;
    };

}

