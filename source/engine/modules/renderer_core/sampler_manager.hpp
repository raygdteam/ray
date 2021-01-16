#pragma once
#include <d3d12.h>

class SamplerDesc :
	public D3D12_SAMPLER_DESC
{
public:
	SamplerDesc()
	{
		Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
		AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		MipLODBias = 0.0f;
		MaxAnisotropy = 0;
		ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		BorderColor[0] = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		BorderColor[1] = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		BorderColor[2] = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		BorderColor[3] = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
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
