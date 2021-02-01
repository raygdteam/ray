#pragma once

#include "gpu_resource.hpp"

struct GpuBufferDescription : public GpuResourceDescription
{
public:
	GpuBufferDescription()
		: GpuResourceDescription()
	{}

	GpuBufferDescription(u32 sizeInBytes, u32 stride, const void* initialData, DXGI_FORMAT format)
		: GpuBufferDescription()
	{
		SizeInBytes = sizeInBytes;
		Stride = stride;
		UploadBufferData = initialData;
		Format = format;
		Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	}

public:
	static GpuBufferDescription Buffer(u32 sizeInBytes, u32 stride = 0, const void* data = nullptr, DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN) noexcept
	{
		return GpuBufferDescription(sizeInBytes, stride, data, format);
	}

	static GpuBufferDescription Vertex(u32 elementsCount, u32 stride, const void* data = nullptr) noexcept
	{
		return Buffer(elementsCount * stride, stride, data);
	}

	static GpuBufferDescription Index(u32 elementsCount, u32 stride, const void* data = nullptr) noexcept
	{
		return Buffer(elementsCount * stride, stride, data, DXGI_FORMAT_UNKNOWN);
	}

	// TODO: Typed, Argument, Raw, Structured

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

