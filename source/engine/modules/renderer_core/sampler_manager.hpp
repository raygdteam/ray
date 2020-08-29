#pragma once
#include <d3d12.h>

namespace ray::renderer_core_api
{

    class SamplerDesc :
        public D3D12_SAMPLER_DESC
    {
    public:
        SamplerDesc()
        {
            Filter = D3D12_FILTER_ANISOTROPIC;
            AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
            AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
            AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
            MipLODBias = 0.0f;
            MaxAnisotropy = 16;
            ComparisonFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
            BorderColor[0] = 1.0f;
            BorderColor[1] = 1.0f;
            BorderColor[2] = 1.0f;
            BorderColor[3] = 1.0f;
            MinLOD = 0.0f;
            MaxLOD = D3D12_FLOAT32_MAX;
        }

        void SetTextureAddressMode(D3D12_TEXTURE_ADDRESS_MODE addressMode)
        {
            AddressU = addressMode;
            AddressV = addressMode;
            AddressW = addressMode;
        }

        void SetBorderColor(float* border)
        {
            BorderColor[0] = border[0];
            BorderColor[1] = border[1];
            BorderColor[2] = border[2];
            BorderColor[3] = border[3];
        }

        // Allocate new descriptor as needed
        D3D12_CPU_DESCRIPTOR_HANDLE CreateDescriptor();

        // Create descriptor in place (no deduplication)
        void CreateDescriptor(D3D12_CPU_DESCRIPTOR_HANDLE& handle);
    };

}
