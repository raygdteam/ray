#include "resources_loader.hpp"
#include "graphics_memory_manager.hpp"
#include "resources_table.hpp"
#include "renderer_core/renderer.hpp"
#include "gpu_allocator.hpp"

#include <core/math/common.hpp>

using namespace ray::core::math;

namespace ray::renderer_core_api::resources
{

	void ResourcesLoader::Initialize() noexcept
	{
		auto mem = globals::gGpuAllocator.AllocateUploadHeap(MAX_POOL_SIZE);
		_underlyingResource = mem.Resource;

		CD3DX12_RANGE range(0, 0);
		_underlyingResource->Map(0, &range, reinterpret_cast<void**>(_begin));

		_currentPointer = _begin;
		_end = static_cast<u8*>(_begin) + MAX_POOL_SIZE;
	}

	void ResourcesLoader::SetBufferData(void* buffer, size_t bufferSize, size_t elementSize) noexcept
	{
		SetDataToUploadBuffer(buffer, bufferSize, elementSize);
	}

	void ResourcesLoader::SetConstantBufferData(void* buffer, size_t bufferSize) noexcept
	{
		SetDataToUploadBuffer(buffer, bufferSize, 256);
	}

	void ResourcesLoader::SetTextureData(RTexture& texture) noexcept
	{
		D3D12_SUBRESOURCE_FOOTPRINT subresourceFootprint;
		subresourceFootprint.Width = texture.GetDimensions().x;
		subresourceFootprint.Height = texture.GetDimensions().y;
		subresourceFootprint.Format = DXGI_FORMAT_R32G32B32A32_UINT;
		subresourceFootprint.RowPitch = AlignUp(subresourceFootprint.Width * 16, 256);

		_currentPointer = reinterpret_cast<u8*>(AlignUp(reinterpret_cast<size_t>(_currentPointer), 512));
		ray_assert(static_cast<u8*>(_currentPointer) + subresourceFootprint.RowPitch * subresourceFootprint.Height <= _end, "Out of upload buffer!")

		AllocatedResource resource;
		resource.Heap = nullptr;
		resource.Resource = _underlyingResource;
		resource.Offset = static_cast<u8*>(_currentPointer) - _begin;
		globals::gLoadedResourcesTable.SetResource(resource, texture.GetId(), RESOURCE_DIMENSION_BUFFER);

		D3D12_PLACED_SUBRESOURCE_FOOTPRINT placedSubres;
		placedSubres.Footprint = subresourceFootprint;
		placedSubres.Offset = static_cast<u64>(static_cast<u8*>(_currentPointer) - _begin);

		const void* textureData = static_cast<const void*>(texture.GetData().GetData());
		for (size_t i = 0; i < subresourceFootprint.Height; ++i)
		{
			u8* dest = static_cast<u8*>(_begin) + placedSubres.Offset + i * subresourceFootprint.RowPitch;
			const u8* src = static_cast<const u8*>(textureData) + i * subresourceFootprint.Width;
			memcpy(dest, src, 16 * subresourceFootprint.Width);
		}
	}

	void ResourcesLoader::SetDataToUploadBuffer(void* buffer, size_t bufferSize, size_t alignment) noexcept
	{
		_currentPointer = reinterpret_cast<u8*>(AlignUp(reinterpret_cast<size_t>(_currentPointer), alignment));
		ray_assert(static_cast<u8*>(_currentPointer) + bufferSize <= _end, "Out of upload buffer!")

		auto byteOffset = static_cast<u64>(static_cast<u8*>(_currentPointer) - _begin);

		AllocatedResource resource;
		resource.Heap = nullptr;
		resource.Resource = _underlyingResource;
		resource.Offset = byteOffset;

		globals::gLoadedResourcesTable.SetBuffer(resource);

		memcpy(_currentPointer, buffer, bufferSize);
		_currentPointer = static_cast<u8*>(_currentPointer) + bufferSize;
	}

}