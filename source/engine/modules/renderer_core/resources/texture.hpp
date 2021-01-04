#pragma once
#include "gpu_resource.hpp"
#include <d3d12.h>
#include <core/memory/memory_pool.hpp>
#include <core/memory/memory_manager.hpp>

namespace ray::renderer_core_api::resources
{
	class TexturePool : public ray::IMemoryPool
	{
		friend class TextureAllocator;

	public:
		~TexturePool() {}

		void Create(size_t maxMemoryPoolSize) noexcept override;
		void Destroy() noexcept override;

	private:
		ID3D12Heap* _heap;

	};

	class TextureAllocator
	{
	public:
		TextureAllocator(size_t preferredSize)
		{
			_memoryManager.Initialize(preferredSize);
		}

		ID3D12Resource* Allocate(u64 width, u64 height) noexcept;
		void Free(ID3D12Resource* resource) noexcept;

	private:
		ray::MemoryManager<TexturePool> _memoryManager;
		TexturePool* _pool = nullptr;

	};

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