#pragma once
#include <d3d12.h>

#include <core/core.hpp>
#include <core/memory/memory_manager.hpp>
#include "gpu_memory_pool.hpp"
#include <renderer_core/descriptor_heap.hpp>

namespace ray::renderer_core_api
{
	class CommandContext;
	class ComputeContext;
	class GraphicsContext;
}

namespace ray::renderer_core_api::resources 
{
	enum class ResourceMappingMode
	{
		eReadAccess,
		eWriteAccess,
	};

	struct GpuResourceDescription
	{
	public:
		// buffer properties
		u32 SizeInBytes;
		u32 Stride;

		// texture properties
		D3D12_RESOURCE_DIMENSION Dimension;
		u32 Width;
		u32 Height;
		u32 Depth;
		u32 ArraySize;
		DXGI_SAMPLE_DESC SampleDesc;
		u32 MipLevels;
		D3D12_RESOURCE_FLAGS Flags;

		// common properties
		DXGI_FORMAT Format;
		const void* UploadBufferData;

	public:
		GpuResourceDescription() = default;

		GpuResourceDescription(GpuResourceDescription&& rhs) = default;
		GpuResourceDescription& operator = (GpuResourceDescription&& rhs) = default;

		GpuResourceDescription(const GpuResourceDescription& lhs) = default;
		GpuResourceDescription& operator = (const GpuResourceDescription& lhs) = default;
	};

	class GpuResource;

	class GpuResourceAllocator
	{
	protected:
		MemoryManager<GpuMemoryPool> _memoryManager;
		GpuMemoryPool* _currentPool;

	public:
		GpuResourceAllocator() = default;

		GpuResourceAllocator(GpuResourceAllocator&& rhs) = default;
		GpuResourceAllocator& operator = (GpuResourceAllocator&& rhs) = default;

		virtual void Initialize(size_t preferredSize) noexcept
		{
			_memoryManager.Initialize(preferredSize);
		}

		virtual void Destroy() noexcept
		{
			_memoryManager.Destroy();
		}

		virtual GpuResource&& Allocate(GpuResourceDescription& desc) noexcept = 0;
		virtual void Free(GpuResource& resource) noexcept = 0;

	};

	class GpuResource
	{
		friend class ray::renderer_core_api::CommandContext;
		friend class ::ray::renderer_core_api::GraphicsContext;
		friend class ::ray::renderer_core_api::ComputeContext;
		friend GpuResourceAllocator;
		friend class ray::renderer_core_api::resources::GpuTextureAllocator;

	protected:
		ID3D12Resource* _resource;
		D3D12_RESOURCE_STATES _usageState;
		D3D12_RESOURCE_STATES _transitioningState;
		u64 _resourceSize;
		GpuMemoryPool* _underlyingPool;
		GpuResourceDescription _desc;

	public:
		GpuResource() noexcept = default;

		GpuResource(ID3D12Resource* inResource, D3D12_RESOURCE_STATES currentState, u64 resourceSize, GpuMemoryPool* pool, GpuResourceDescription& desc) noexcept
			: _resource(inResource)
			, _usageState(currentState)
			, _transitioningState(static_cast<D3D12_RESOURCE_STATES>(-1))
			, _resourceSize(resourceSize)
			, _underlyingPool(pool)
			, _desc(desc)
		{}

		GpuResource(GpuResource&& rhs) = default;
		GpuResource& operator = (GpuResource&& rhs) = default;

		virtual ~GpuResource() 
		{
			Destroy();
		}

	public:
		// destroys and releases allocated memory
		void Destroy() noexcept
		{
			if (_resourceSize != 0)
			{
				Release();
				_resourceSize = 0;
			}
		}

		void Map(ResourceMappingMode mode, void** data) const noexcept
		{
			D3D12_RANGE range;
			range.Begin = 0;
			range.End = 0;

			D3D12_RANGE* access = &range;

			if (mode == ResourceMappingMode::eReadAccess)
				access = nullptr;

			_resource->Map(0, access, data);
		}

		void Unmap() const noexcept
		{
			_resource->Unmap(0, nullptr);
		}

	public:
		// loads data to allocated memory in gpu.
		virtual bool Load(const void* initialData) noexcept { return initialData != nullptr; }

		// called when resource is being destroyed. Release() must clean up allocated memory 
		// by calling GpuResourceAllocator::Free.
		virtual void Release() noexcept {}

	public:
		ID3D12Resource* GetNativeResource() const noexcept { return _resource; }
		u64 GetResourceSize() const noexcept { return _resourceSize; }
		GpuMemoryPool* GetUnderlyingPool() const noexcept { return _underlyingPool; }
		GpuResourceDescription GetDesc() const noexcept { return _desc; }

	};

	class IResourceView
	{
	protected:
		DescriptorHandle _srvHandle;
		DescriptorHandle _uavHandle;

	public:
		virtual void Create(GpuResource& resource) noexcept = 0;

	public:
		D3D12_CPU_DESCRIPTOR_HANDLE GetSRV() const noexcept
		{
			return _srvHandle.GetCpuHandle();
		}

		D3D12_CPU_DESCRIPTOR_HANDLE GetUAV() const noexcept
		{
			return _uavHandle.GetCpuHandle();
		}

	};

}