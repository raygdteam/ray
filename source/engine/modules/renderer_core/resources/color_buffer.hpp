#pragma once
#include "gpu_pixel_buffer.hpp"
#include <cassert>

#pragma clang diagnostic ignored "-Wpessimizing-move"

class ColorBuffer : public GpuPixelBuffer
{
private:
	float _clearColor[4]; // TODO: class Color

public:
	ColorBuffer(float r = 1.f, float g = 1.f, float b = 1.f, float a = 1.f)
	{
		_clearColor[0] = r;
		_clearColor[1] = g;
		_clearColor[2] = b;
		_clearColor[3] = a;
	}

public:
	void CreateFromSwapChain(ID3D12Resource* inResource, pcstr debugName);
	void Create(u32 width, u32 height, u32 numMips, DXGI_FORMAT format, D3D12_RESOURCE_FLAGS flags, pcstr debugName);
	void CreateArray(u32 width, u32 height, u32 numMips, u32 arrayCount, DXGI_FORMAT format, D3D12_RESOURCE_FLAGS flags, pcstr debugName);
	void Reset(u32 width, u32 height, pcstr debugName) noexcept;

public:
	D3D12_CPU_DESCRIPTOR_HANDLE GetSRV() const noexcept { return _view.GetSRV(); }
	D3D12_CPU_DESCRIPTOR_HANDLE GetRTV() const noexcept { return _view.GetRTV(); }
	D3D12_CPU_DESCRIPTOR_HANDLE GetUAV() const noexcept { return _view.GetUAV(); }

	TextureView& GetTextureView() noexcept
	{
		return _view;
	}

	void GetClearColor(float* r, float* g, float* b, float* a) const noexcept
	{
		*r = _clearColor[0];
		*g = _clearColor[1];
		*b = _clearColor[2];
		*a = _clearColor[3];
	}

	void SetClearColor(float r, float g, float b, float a) noexcept
	{
		_clearColor[0] = r;
		_clearColor[1] = g;
		_clearColor[2] = b;
		_clearColor[3] = a;
	}

public:
	void GenerateMipMaps(CommandContext& context);

protected:
	static inline u32 ComputeNumMips(u32 width, u32 height)
	{
		u32 highBit;
		_BitScanReverse((DWORD*)&highBit, width | height);
		return highBit + 1;
	}

};
