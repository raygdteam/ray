#pragma once
#include "pixel_buffer.hpp"

namespace ray::renderer_core_api::resources
{
	class ColorBuffer : public PixelBuffer
	{
	public:
		ColorBuffer(float r, float g, float b, float a)
			: _numMipMaps(0)
			, _fragmentCount(1)
			, _sampleCount(1)
		{
			_clearColor[0] = r;
			_clearColor[1] = g;
			_clearColor[2] = b;
			_clearColor[3] = a;

			_rtvHandle.ptr = static_cast<D3D12_GPU_VIRTUAL_ADDRESS>(-1);
			_srvHandle.ptr = static_cast<D3D12_GPU_VIRTUAL_ADDRESS>(-1);
			std::memset(_uavHandle, 0xff, sizeof(_uavHandle));
		}

		void CreateFromSwapChain(ID3D12Resource* inResource);
		void Create(u32 width, u32 height, u32 numMips, DXGI_FORMAT format, D3D12_GPU_VIRTUAL_ADDRESS address);
		void CreateArray(u32 width, u32 height, u32 numMips, u32 arrayCount, DXGI_FORMAT format, D3D12_GPU_VIRTUAL_ADDRESS address);

		const D3D12_CPU_DESCRIPTOR_HANDLE& GetSRV() const noexcept { return _srvHandle; }
		const D3D12_CPU_DESCRIPTOR_HANDLE& GetRTV() const noexcept { return _rtvHandle; }
		const D3D12_CPU_DESCRIPTOR_HANDLE& GetUAV() const noexcept { return _uavHandle[0]; }

		void SetClearColor(float r, float g, float b, float a) noexcept 
		{ 
			_clearColor[0] = r;
			_clearColor[1] = g;
			_clearColor[2] = b;
			_clearColor[3] = a;
		}

		// Msaa - multisample anti-aliasing
		void SetMsaaMode(u32 numColorSamples, u32 numCoverageSamples)
		{
			assert(numCoverageSamples >= numColorSamples);
			_fragmentCount = numColorSamples;
			_sampleCount = numCoverageSamples;
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
		D3D12_RESOURCE_FLAGS CombineResourceFlags() const
		{
			D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE;
			if (_fragmentCount == 1)
				flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

			return flags | D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
		}

		static inline u32 ComputeNumMips(u32 width, u32 height)
		{
			u32 highBit;
			_BitScanReverse((DWORD*)&highBit, width | height);
			return highBit + 1;
		}

		void CreateDerivedViews(ID3D12Device* device, DXGI_FORMAT format, u32 arraySize, u32 numMips = 1);

	protected:
		float _clearColor[4]; // TODO: class Color
		D3D12_CPU_DESCRIPTOR_HANDLE _rtvHandle;
		D3D12_CPU_DESCRIPTOR_HANDLE _srvHandle;
		D3D12_CPU_DESCRIPTOR_HANDLE _uavHandle[12];
		u32 _numMipMaps;
		u32 _fragmentCount;
		u32 _sampleCount;

	};
}