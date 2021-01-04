#pragma once
#include "gpu_resource.hpp"
#include <d3d12.h>
#include <core/core.hpp>
#include <core/memory/memory_pool.hpp>
#include <core/memory/memory_manager.hpp>

namespace ray::renderer_core_api::resources
{
	struct GpuTextureDescription
	{
	public:
		D3D12_RESOURCE_DIMENSION Dimension;
		u32 Width;
		u32 Height;
		u32 Depth;
		u32 ArraySize;
		DXGI_FORMAT Format;
		DXGI_SAMPLE_DESC SampleDesc;
		u32 MipLevels;
		D3D12_RESOURCE_FLAGS Flags;

	public:
		GpuTextureDescription() = default;
		~GpuTextureDescription() = default;

		GpuTextureDescription(GpuTextureDescription&& rhs) = default;
		GpuTextureDescription& operator = (GpuTextureDescription&&) = default;

	public:
		static GpuTextureDescription Texture1D(u32 width, DXGI_FORMAT format, u32 arraySize, D3D12_RESOURCE_FLAGS flags) noexcept
		{
			GpuTextureDescription desc;
			desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE1D;
			desc.Width = width;
			desc.Height = 1;
			desc.Depth = 1;
			desc.ArraySize = arraySize;
			desc.MipLevels = 1;
			desc.Format = format;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Flags = flags;
			return desc;
		}

		static GpuTextureDescription Texture2D(u32 width, u32 height, DXGI_FORMAT format, u32 arraySize, D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET) noexcept
		{
			GpuTextureDescription desc;
			desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			desc.Width = width;
			desc.Height = height;
			desc.Depth = 1;
			desc.ArraySize = arraySize;
			desc.MipLevels = 1;
			desc.Format = format;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Flags = flags;
			return desc;
		}

		static GpuTextureDescription Texture3D(u32 width, u32 height, u32 depth, DXGI_FORMAT format, D3D12_RESOURCE_FLAGS flags) noexcept
		{
			GpuTextureDescription desc;
			desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE3D;
			desc.Width = width;
			desc.Height = height;
			desc.Depth = depth;
			desc.ArraySize = 1;
			desc.MipLevels = 1;
			desc.Format = format;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Flags = flags;
			return desc;
		}

	};

	class GpuTexture;

	class TexturePool : public ray::IMemoryPool
	{
		friend class TextureAllocator;

	public:
		~TexturePool() override {}

		void Create(size_t maxMemoryPoolSize, size_t index) noexcept override;
		void Destroy() noexcept override;

	private:
		ID3D12Heap* _heap;

	};

	class TextureAllocator
	{
	public:
		TextureAllocator() = default;

		void Initialize(size_t preferredSize) noexcept
		{
			_memoryManager.Initialize(preferredSize);
		}

		void Destroy() noexcept
		{
			_memoryManager.Destroy();
		}

		GpuTexture&& Allocate(GpuTextureDescription& textureDesc) noexcept;
		void Free(GpuTexture& resource) noexcept;

	private:
		ray::MemoryManager<TexturePool> _memoryManager;
		TexturePool* _pool = nullptr;

	};

	// represents texture resource in gpu memory
	class GpuTexture : public GpuResource
	{
		friend TextureAllocator;

	private:
		GpuTextureDescription _desc;
		TexturePool* _underlyingPool;

	public:
		GpuTexture() = default;
		GpuTexture(GpuTexture&& rhs) = default;
		GpuTexture& operator = (GpuTexture&& rhs) = default;

		~GpuTexture() override {}

		bool Create(GpuTextureDescription& desc) noexcept;

		void Release() noexcept override;

	};

	size_t BitsPerPixel(_In_ DXGI_FORMAT fmt) noexcept;
	size_t BitesPerPixel(DXGI_FORMAT fmt) noexcept
	{
		return (BitsPerPixel(fmt) / 8);
	}
}