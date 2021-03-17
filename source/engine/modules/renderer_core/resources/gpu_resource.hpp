#pragma once
#include <d3d12.h>

#include <core/core.hpp>
#include <core/lib/string.hpp>
#include <core/memory/memory_manager.hpp>
#include "gpu_memory_pool.hpp"
#include <renderer_core/descriptor_heap.hpp>
#include <renderer_core/renderer_core.hpp>

class UploadBuffer;

enum class ResourceMappingMode
{
	eReadAccess,
	eWriteAccess,
};

enum class GpuBufferType : u8
{
	eIndex = 0,
	eVertex = 1,
	eStructured = 2,
	eByteAddress = 3,
};

struct RAY_RENDERERCORE_API GpuResourceDescription
{
public:
	// texture properties
	u32 Width;
	u32 Height;
	u32 Depth;
	u32 ArraySize;
	DXGI_SAMPLE_DESC SampleDesc;
	u32 MipLevels;
	D3D12_CLEAR_VALUE* ClearValue;

	// buffer properties
	GpuBufferType Type;
	u32 SizeInBites;
	u32 Stride;

	// common properties
	D3D12_RESOURCE_DIMENSION Dimension;
	DXGI_FORMAT Format;
	const void* UploadBufferData;
	D3D12_RESOURCE_FLAGS Flags;

public:
	GpuResourceDescription
	(
		D3D12_RESOURCE_DIMENSION dimension = D3D12_RESOURCE_DIMENSION_UNKNOWN, 
		DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN, 
		D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE, 
		const void* uploadBufferData = nullptr
	)
		: Dimension(dimension)
		, Format(format)
		, Flags(flags)
		, UploadBufferData(uploadBufferData)
	{}

	GpuResourceDescription(GpuResourceDescription&& rhs) = default;
	GpuResourceDescription& operator = (GpuResourceDescription&& rhs) = default;

	GpuResourceDescription(const GpuResourceDescription& lhs) = default;
	GpuResourceDescription& operator = (const GpuResourceDescription& lhs) = default;
};

class GpuResource;

template <typename TGpuMemoryPool>
class RAY_RENDERERCORE_API GpuResourceAllocator
{
protected:
	MemoryManager<TGpuMemoryPool> _memoryManager;
	TGpuMemoryPool* _currentPool;

public:
	GpuResourceAllocator() = default;

	GpuResourceAllocator(GpuResourceAllocator&& rhs) = default;
	GpuResourceAllocator& operator = (GpuResourceAllocator&& rhs) = default;

	virtual void Initialize(size_t preferredSize) noexcept
	{
		_memoryManager.Initialize(preferredSize);
		_currentPool = &_memoryManager.RequestPool(preferredSize);
	}

	virtual void Destroy() noexcept
	{
		_memoryManager.Destroy();
	}

	NODISCARD virtual GpuResource&& Allocate(GpuResourceDescription& desc) noexcept = 0;
	virtual void Free(GpuResource& resource) noexcept = 0;

};

class RAY_RENDERERCORE_API GpuResource
{
	friend class CommandContext;
	friend class GraphicsContext;
	friend class ComputeContext;
	friend GpuResourceAllocator<GpuBufferMemoryPool>;
	friend GpuResourceAllocator<GpuTextureMemoryPool>;
	friend GpuResourceAllocator<GpuPixelBufferMemoryPool>;
	friend class GpuTextureAllocator;

protected:
	ID3D12Resource* _resource;
	D3D12_RESOURCE_STATES _usageState;
	D3D12_RESOURCE_STATES _transitioningState;
	u64 _resourceSize;
	ray::IMemoryPool* _underlyingPool;
	GpuResourceDescription _desc;
	bool _bManaged;
	String _debugName;

public:
	GpuResource() 
		: _resource(nullptr)
		, _usageState(D3D12_RESOURCE_STATE_COMMON)
		, _transitioningState(static_cast<D3D12_RESOURCE_STATES>(-1))
		, _resourceSize(0)
		, _underlyingPool(nullptr)
		, _bManaged(true)
		, _debugName("GpuResource::Unnamed")
	{}

	GpuResource(ID3D12Resource* inResource, D3D12_RESOURCE_STATES currentState, u64 resourceSize, GpuMemoryPool* pool, GpuResourceDescription& desc, bool bManaged = true) noexcept
		: _resource(inResource)
		, _usageState(currentState)
		, _transitioningState(static_cast<D3D12_RESOURCE_STATES>(-1))
		, _resourceSize(resourceSize)
		, _underlyingPool(pool)
		, _desc(desc)
		, _bManaged(true)
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
	// loads data to allocated memory in gpu from upload buffer.
	virtual bool Load(UploadBuffer& uploadBuffer, const void* initialData) noexcept { return initialData != nullptr; }

	// called when resource is being destroyed. Release() must clean up allocated memory 
	// by calling GpuResourceAllocator::Free.
	virtual void Release() noexcept {}

public:
	ID3D12Resource* GetNativeResource() const noexcept { return _resource; }
	u64 GetResourceSize() const noexcept { return _resourceSize; }
	ray::IMemoryPool* GetUnderlyingPool() const noexcept { return _underlyingPool; }
	GpuResourceDescription GetDesc() const noexcept { return _desc; }
	// whether resource is managed by allocator
	bool IsManaged() const noexcept { return _bManaged; }

};

class RAY_RENDERERCORE_API IResourceView
{
protected:
	DescriptorHandle _srvHandle;
	DescriptorHandle _uavHandle;

	DescriptorHeap* _cbvSrvUavHeap;

public:
	virtual void Create(GpuResource& resource, DescriptorHeap* cbvSrvUavHeap = nullptr, DescriptorHeap* rtvHeap = nullptr, DescriptorHeap* dsvHeap = nullptr) noexcept = 0;

public:
	D3D12_CPU_DESCRIPTOR_HANDLE GetSRV() const noexcept
	{
		return _srvHandle.GetCpuHandle();
	}

	D3D12_CPU_DESCRIPTOR_HANDLE GetUAV() const noexcept
	{
		return _uavHandle.GetCpuHandle();
	}

	DescriptorHeap* GetCBV_SRV_UAV_Heap() const noexcept
	{
		return _cbvSrvUavHeap;
	}

};
