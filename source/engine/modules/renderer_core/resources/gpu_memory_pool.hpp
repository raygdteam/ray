#pragma once 
#include <core/core.hpp>
#include <core/memory/memory_pool.hpp>
#include <renderer_core/dx12_helper_functions.hpp>
#include <d3d12.h>

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

class GpuTextureMemoryPool : public ray::IMemoryPool
{
	friend class GpuTextureAllocator;

private:
	ID3D12Heap* _heap;

public:
	~GpuTextureMemoryPool() override {}

public:
	void Create(size_t maxMemoryPoolSize, size_t index) noexcept override;
	void Destroy() noexcept override;

};

class GpuBufferMemoryPool : public ray::IMemoryPool
{
	friend class GpuBufferAllocator;

private:
	ID3D12Heap* _heap;

public:
	~GpuBufferMemoryPool() override {}

public:
	void Create(size_t maxMemoryPoolSize, size_t index) noexcept override;
	void Destroy() noexcept override;

};

class GpuPixelBufferMemoryPool : public ray::IMemoryPool
{
	friend class GpuPixelBufferAllocator;

private:
	ID3D12Heap* _heap;

public:
	~GpuPixelBufferMemoryPool() override {}

public:
	void Create(size_t maxMemoryPoolSize, size_t index) noexcept override;
	void Destroy() noexcept override;

};
