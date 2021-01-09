#pragma once 
#include <core/core.hpp>
#include <core/memory/memory_pool.hpp>
#include <renderer_core/dx12_helper_functions.hpp>
#include <d3d12.h>

namespace ray::renderer_core_api::resources
{
	class TextureAllocator;

	class GpuMemoryPool : public ray::IMemoryPool
	{
		friend class GpuTextureAllocator;
		friend class GpuBufferAllocator;

	private:
		ID3D12Heap* _heap;

	public:
		~GpuMemoryPool() override {}

	public:
		void Create(size_t maxMemoryPoolSize, size_t index) noexcept override;
		void Destroy() noexcept override;

	};
}

