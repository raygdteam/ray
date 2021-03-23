#pragma once 
#include <core/core.hpp>
#include <core/memory/memory_pool.hpp>
#include <core/memory/pool_allocator.hpp>
#include <renderer_core/dx12_helper_functions.hpp>
#include <d3d12.h>

class GpuMemoryPool : public IMemoryPool
{
private:
	ID3D12Heap* _heap;
	PoolAllocator _allocator;

public:
	GpuMemoryPool()
		: IMemoryPool(0, 0)
	{}

	GpuMemoryPool(size_t poolSize, size_t poolIndex, D3D12_HEAP_FLAGS flags) noexcept;
	~GpuMemoryPool() override {}

public:
	void Destroy() noexcept override;
	bool IsEnough(size_t size) const noexcept override;
	void* GetPool() const noexcept override
	{
		return _heap;
	}

	size_t Allocate(size_t size) noexcept override;
	void Free(size_t offset, size_t size) noexcept override;

};

class GpuTextureMemoryPool : public GpuMemoryPool
{
	friend class GpuResourceAllocator<GpuTextureMemoryPool>;

public:
	GpuTextureMemoryPool()
		: GpuMemoryPool()
	{}

	GpuTextureMemoryPool(size_t poolSize, size_t poolIndex);
	~GpuTextureMemoryPool() override {}

};

class GpuBufferMemoryPool : public GpuMemoryPool
{
	friend class GpuResourceAllocator<GpuBufferMemoryPool>;

public:
	GpuBufferMemoryPool()
		: GpuMemoryPool()
	{}

	GpuBufferMemoryPool(size_t poolSize, size_t poolIndex);
	~GpuBufferMemoryPool() override {}

};

class GpuPixelBufferMemoryPool : public GpuMemoryPool
{
	friend class GpuResourceAllocator<GpuPixelBufferMemoryPool>;

public:
	GpuPixelBufferMemoryPool()
		: GpuMemoryPool()
	{}

	GpuPixelBufferMemoryPool(size_t poolSize, size_t poolIndex);
	~GpuPixelBufferMemoryPool() override {}

};
