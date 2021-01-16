#pragma once
#include "gpu_texture.hpp"
#include <cassert>

class ColorBuffer : public GpuTexture
{
protected:
	float _clearColor[4]; // TODO: class Color
	TextureView _view;

public:
	ColorBuffer(float r, float g, float b, float a)
	{
		_clearColor[0] = r;
		_clearColor[1] = g;
		_clearColor[2] = b;
		_clearColor[3] = a;
	}

	void CreateFromSwapChain(ID3D12Resource* inResource);
	void Create(u32 width, u32 height, u32 numMips, DXGI_FORMAT format, D3D12_RESOURCE_FLAGS flags);
	void CreateArray(u32 width, u32 height, u32 numMips, u32 arrayCount, DXGI_FORMAT format, D3D12_RESOURCE_FLAGS flags);

	const D3D12_CPU_DESCRIPTOR_HANDLE& GetSRV() const noexcept { return _view.GetSRV(); }
	const D3D12_CPU_DESCRIPTOR_HANDLE& GetRTV() const noexcept { return _view.GetRTV(); }
	const D3D12_CPU_DESCRIPTOR_HANDLE& GetUAV() const noexcept { return _view.GetUAV(); }

	void SetClearColor(float r, float g, float b, float a) noexcept
	{
		_clearColor[0] = r;
		_clearColor[1] = g;
		_clearColor[2] = b;
		_clearColor[3] = a;
	}


	void GetClearColor(float* r, float* g, float* b, float* a) const noexcept
	{
		*r = _clearColor[0];
		*g = _clearColor[1];
		*b = _clearColor[2];
		*a = _clearColor[3];
	}

	void GenerateMipMaps(CommandContext& context);

protected:
	static inline u32 ComputeNumMips(u32 width, u32 height)
	{
		u32 highBit;
		_BitScanReverse((DWORD*)&highBit, width | height);
		return highBit + 1;
	}

};
