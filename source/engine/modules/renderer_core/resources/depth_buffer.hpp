#pragma once
#include "gpu_pixel_buffer.hpp"

class DepthBuffer : public GpuPixelBuffer
{
public:
	DepthBuffer() = default;

public:
	void Create(u32 width, u32 height, DXGI_FORMAT format, pcstr debugName);

public:
	D3D12_CPU_DESCRIPTOR_HANDLE GetDSV() const { return _view.GetDSV(); }
	D3D12_CPU_DESCRIPTOR_HANDLE GetDSV_Readonly() const { return _view.GetDSV_ReadOnly(); }

};
