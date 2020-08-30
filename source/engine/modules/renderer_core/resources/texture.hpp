#pragma once
#include "gpu_resource.hpp"


namespace ray::renderer_core_api::resources
{
	class Texture : public GpuResource
	{
	public:
		Texture()
		{
			_CpuHandle.ptr = static_cast<D3D12_GPU_VIRTUAL_ADDRESS>(-1);
		}

		Texture(D3D12_CPU_DESCRIPTOR_HANDLE handle)
			: _CpuHandle(handle)
		{}

		void Create(size_t pitch, size_t width, size_t height, DXGI_FORMAT format, const void* initialData);
		void Create(size_t width, size_t height, DXGI_FORMAT format, const void* initialData)
		{
			return Create(width, width, height, format, initialData);
		}

		D3D12_CPU_DESCRIPTOR_HANDLE GetSRV() const noexcept
		{
			return _CpuHandle;
		}

		void Destroy() noexcept override
		{
			GpuResource::Destroy();
			// TODO: reusing freed descriptors
			_CpuHandle.ptr = static_cast<D3D12_GPU_VIRTUAL_ADDRESS>(0);
		}

		bool operator!() const noexcept { return _CpuHandle.ptr == static_cast<D3D12_GPU_VIRTUAL_ADDRESS>(0); }

	protected:
		D3D12_CPU_DESCRIPTOR_HANDLE _CpuHandle;

	};

	size_t BitsPerPixel(_In_ DXGI_FORMAT fmt);
}