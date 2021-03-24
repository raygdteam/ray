#pragma once
#include <core/core.hpp>

template<typename TGpuMemoryPool>
class GpuResourceAllocator;

class GpuTextureMemoryPool;
class GpuBufferMemoryPool;
class GpuPixelBufferMemoryPool;

class IMemoryPool
{
	friend class GpuResourceAllocator<GpuTextureMemoryPool>;
	friend class GpuResourceAllocator<GpuBufferMemoryPool>;
	friend class GpuResourceAllocator<GpuPixelBufferMemoryPool>;

protected:
	void* _pool;
	size_t _index;
	size_t _poolSize;

public:
	IMemoryPool() = default;
	IMemoryPool(size_t poolSize, size_t index) noexcept
		: _index(index)
		, _poolSize(poolSize)
	{}

	virtual ~IMemoryPool() {}

public:
	virtual void Destroy() noexcept = 0;

public:
	virtual size_t Allocate(size_t size) noexcept = 0;
	virtual void Free(size_t offset, size_t size) noexcept = 0;

public:
	virtual bool IsEnough(size_t size) const noexcept = 0;

	size_t GetIndex() const noexcept
	{
		return _index;
	}

	virtual void* GetPool() const noexcept = 0;

};