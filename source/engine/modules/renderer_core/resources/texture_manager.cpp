#include "texture_manager.hpp"
#include <renderer_core/d3dx12.h>
#include <renderer_core/renderer.hpp>
#include <renderer_core/resources/texture.hpp>
#include <renderer_core/resources/gpu_resource.hpp>

namespace ray::renderer_core_api
{
	DynAlloc* TextureManager::_uploadedResources[64];
	ManagedTexture TextureManager::_gpuResources[64];


	void TextureManager::PrepareTextures(CommandContext& ctx, RTexture** textures, size_t numTextures, bool bFlush)
	{
		check(numTextures <= 64)

		_descriptorHeap = UserDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 64);

		D3D12_RESOURCE_DESC resourceDesc = {};
		resourceDesc.MipLevels = 1;
		resourceDesc.DepthOrArraySize = 1;
		resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		resourceDesc.SampleDesc.Count = 1;
		resourceDesc.SampleDesc.Quality = 0;
		resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

		D3D12_HEAP_PROPERTIES heapProps = {};
		heapProps.CreationNodeMask = 1;
		heapProps.VisibleNodeMask = 1;
		heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
		resourceDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;

		for (size_t i = 0; i < numTextures; ++i)
		{
			RTexture* texture = textures[i];

			resourceDesc.Width = texture->GetDimensions().x;
			resourceDesc.Height = texture->GetDimensions().y;

			auto hr = globals::gDevice->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc,
				D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&_gpuResources[texture->GetId()].Resource));

			check(hr == S_OK)

			auto textureSize = GetRequiredIntermediateSize(_gpuResources[texture->GetId()].Resource, 0, 1);

			auto mem = ctx.ReserveUploadMemory(textureSize);
			_uploadedResources[texture->GetId()] = new DynAlloc(mem.Buffer, mem.Offset, mem.Size);
			_uploadedResources[texture->GetId()]->GpuVirtualAddress = mem.GpuVirtualAddress;
			
		}

		if (bFlush)
			LoadToGPU(ctx, textures, numTextures);
	}

	void TextureManager::LoadToGPU(CommandContext& ctx, RTexture** textures, size_t numTextures)
	{
		for (size_t i = 0; i < numTextures; ++i)
		{
			RTexture* texture = textures[i];

			D3D12_SUBRESOURCE_DATA data;
			data.pData = texture->GetData().GetData();
			data.RowPitch = texture->GetDimensions().x * (resources::BitsPerPixel(DXGI_FORMAT_R32G32B32A32_UINT) / 8);
			data.SlicePitch = data.RowPitch * texture->GetDimensions().y;

			UpdateSubresources(static_cast<ID3D12GraphicsCommandList*>(ctx.GetCommandList()), _gpuResources[texture->GetId()].Resource,
								_uploadedResources[texture->GetId()]->Buffer.GetResource(), 0, 0, 1, &data);

			resources::GpuResource dest(_gpuResources[texture->GetId()].Resource, D3D12_RESOURCE_STATE_COPY_DEST);
			ctx.TransitionResource(dest, D3D12_RESOURCE_STATE_GENERIC_READ);

			_gpuResources[texture->GetId()].Descriptor = _descriptorHeap.Allocate().GetCpuHandle();

			globals::gDevice->CreateShaderResourceView(_gpuResources[texture->GetId()].Resource, nullptr, _gpuResources[texture->GetId()].Descriptor);
		}

		ctx.Finish(true);
	}

	void TextureManager::Destroy() noexcept
	{
		for (size_t i = 0; i < 64; ++i)
		{
			if (_uploadedResources[i] != nullptr)
				delete _uploadedResources[i];
		}
	}
}