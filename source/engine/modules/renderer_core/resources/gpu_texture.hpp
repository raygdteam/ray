#pragma once
#include "gpu_resource.hpp"
#include <d3d12.h>
#include <core/core.hpp>
#include <core/memory/memory_pool.hpp>
#include <core/memory/memory_manager.hpp>

struct GpuTextureDescription : public GpuResourceDescription
{
public:
	GpuTextureDescription()
		: GpuResourceDescription()
	{}

	GpuTextureDescription(D3D12_RESOURCE_DIMENSION dimension, u32 width, u32 height, u32 depth, u32 arraySize, DXGI_FORMAT format, DXGI_SAMPLE_DESC sampleDesc, u32 mipLevels, D3D12_RESOURCE_FLAGS flags)
	{
		Dimension = dimension;
		Width = width;
		Height = height;
		Depth = depth;
		ArraySize = arraySize;
		Format = format;
		SampleDesc = sampleDesc;
		MipLevels = mipLevels;
		Flags = flags;
		UploadBufferData = nullptr;
	}

public:
	static GpuTextureDescription Texture1D(u32 width, DXGI_FORMAT format, u32 arraySize, D3D12_RESOURCE_FLAGS flags) noexcept
	{
		DXGI_SAMPLE_DESC sampleDesc;
		sampleDesc.Count = 1;
		sampleDesc.Quality = 0;
		return GpuTextureDescription(D3D12_RESOURCE_DIMENSION_TEXTURE1D, width, 1, 1, arraySize, format, sampleDesc, 1, flags);
	}

	static GpuTextureDescription Texture2D(u32 width, u32 height, DXGI_FORMAT format, u32 arraySize, D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET) noexcept
	{
		DXGI_SAMPLE_DESC sampleDesc;
		sampleDesc.Count = 1;
		sampleDesc.Quality = 0;
		return GpuTextureDescription(D3D12_RESOURCE_DIMENSION_TEXTURE2D, width, height, 1, arraySize, format, sampleDesc, 1, flags);
	}

	static GpuTextureDescription Texture3D(u32 width, u32 height, u32 depth, DXGI_FORMAT format, D3D12_RESOURCE_FLAGS flags) noexcept
	{
		DXGI_SAMPLE_DESC sampleDesc;
		sampleDesc.Count = 1;
		sampleDesc.Quality = 0;
		return GpuTextureDescription(D3D12_RESOURCE_DIMENSION_TEXTURE3D, width, height, depth, 1, format, sampleDesc, 1, flags);
	}

};

class GpuTexture;

class GpuTextureAllocator : public GpuResourceAllocator<GpuTextureMemoryPool>
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

	GpuResource&& Allocate(GpuResourceDescription& textureDesc) noexcept override;
	void Free(GpuResource& resource) noexcept override;

};

// represents texture resource in gpu memory
class GpuTexture : public GpuResource
{
	friend GpuTextureAllocator;

public:
	GpuTexture() = default;

	~GpuTexture() override {}

public:
	bool Create(GpuTextureDescription& desc) noexcept;

public:
	bool Load(const void* uploadBufferData) noexcept override;
	void Release() noexcept override;

public:
	static DXGI_FORMAT GetBaseFormat(DXGI_FORMAT format);
	static DXGI_FORMAT GetUAVFormat(DXGI_FORMAT format);
	static DXGI_FORMAT GetDSVFormat(DXGI_FORMAT format);
	static DXGI_FORMAT GetDepthFormat(DXGI_FORMAT format);
	static DXGI_FORMAT GetStencilFormat(DXGI_FORMAT format);
	static size_t BytesPerPixel(DXGI_FORMAT format);

};

class TextureView : public IResourceView
{
private:
	DescriptorHandle _rtvHandle;
	DescriptorHandle _dsvHandle[2];

public:
	void Create(GpuResource& resource) noexcept override;

public:
	D3D12_CPU_DESCRIPTOR_HANDLE GetRTV() const noexcept
	{
		return _rtvHandle.GetCpuHandle();
	}

	D3D12_CPU_DESCRIPTOR_HANDLE GetDSV() const noexcept
	{
		return _dsvHandle[0].GetCpuHandle();
	}

	D3D12_CPU_DESCRIPTOR_HANDLE GetDSV_ReadOnly() const noexcept
	{
		return _dsvHandle[1].GetCpuHandle();
	}

};
