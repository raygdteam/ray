#pragma once

#include "gpu_resource.hpp"

struct GpuBufferDescription : public GpuResourceDescription
{
public:
	GpuBufferDescription() noexcept
		: GpuResourceDescription()
	{}

	GpuBufferDescription
	(
		GpuBufferType type, 
		u32 sizeInBites, 
		u32 stride, 
		DXGI_FORMAT format, 
		D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE, 
		const void* initialData = nullptr
	) noexcept
	{
		Type = type;
		SizeInBites = sizeInBites;
		Stride = stride;
	}

public:
	static GpuBufferDescription Buffer
	(
		GpuBufferType type, 
		u32 sizeInBytes, 
		u32 stride = 0, 
		DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN, 
		D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE, 
		const void* data = nullptr
	) noexcept
	{
		return GpuBufferDescription(type, sizeInBytes, stride, format, flags, data);
	}

	static GpuBufferDescription Vertex(u32 elementsCount, u32 stride, const void* data = nullptr) noexcept
	{
		return Buffer
		(
			GpuBufferType::eVertex, 
			elementsCount * stride, 
			stride,
			DXGI_FORMAT_UNKNOWN, 
			D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE, 
			data
		);
	}

	static GpuBufferDescription Index(u32 elementsCount, u32 stride, const void* data = nullptr) noexcept
	{
		return Buffer
		(
			GpuBufferType::eIndex, 
			elementsCount * stride, 
			stride, 
			DXGI_FORMAT_UNKNOWN, 
			D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE,
			data
		);
	}

	static GpuBufferDescription Structured
	(
		u32 elementsCount, 
		u32 stride, 
		bool bWritable = false, 
		const void* data = nullptr
	) noexcept
	{
		return Buffer
		(
			GpuBufferType::eStructured, 
			elementsCount * stride, 
			stride, 
			DXGI_FORMAT_UNKNOWN, 
			bWritable ? 
			D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS : 
			D3D12_RESOURCE_FLAG_NONE, 
			data
		);
	}

};

class GpuBufferAllocator : public GpuResourceAllocator<GpuBufferMemoryPool>
{
public:
	void Initialize(size_t preferredSize) noexcept override
	{
		GpuResourceAllocator::Initialize(preferredSize);
	}

	void Destroy() noexcept override
	{
		GpuResourceAllocator::Destroy();
	}

	NODISCARD GpuResource&& Allocate(GpuResourceDescription& bufferDesc) noexcept override;
	void Free(GpuResource& resource) noexcept override;

};

// represents buffer resource in gpu memory
class GpuBuffer : public GpuResource
{
	friend struct IRenderer;
	friend struct Renderer2DData;
	friend GpuBufferAllocator;

public:
	GpuBuffer() = default;

	GpuBuffer(GpuBuffer&& rhs) = default;
	GpuBuffer& operator = (GpuBuffer&& rhs) = default;

	~GpuBuffer() override {}

public:
	void Create(GpuBufferDescription& desc, pcstr debugName) noexcept;

public:
	bool Load(UploadBuffer& uploadBuffer, const void* initialData) noexcept override;
	void Release() noexcept override;

};

class BufferView : public IResourceView
{
private:
	D3D12_VERTEX_BUFFER_VIEW _vbView;
	D3D12_INDEX_BUFFER_VIEW _ibView;
	D3D12_GPU_VIRTUAL_ADDRESS _cbView;

public:
	void Create(GpuResource& resource, DescriptorHeap* cbvSrvUavHeap = nullptr, DescriptorHeap* rtvHeap = nullptr, DescriptorHeap* dsvHeap = nullptr) noexcept override;

public:
	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView() const noexcept
	{
		return _vbView;
	}

	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView() const noexcept
	{
		return _ibView;
	}

	D3D12_GPU_VIRTUAL_ADDRESS GetContantBufferView() const noexcept
	{
		return _cbView;
	}

};

