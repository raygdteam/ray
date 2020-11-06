#include "resources_loader.hpp"
#include "graphics_memory_manager.hpp"
#include "resources_table.hpp"
#include "renderer_core/renderer.hpp"


#include <core/math/common.hpp>

using namespace ray::core::math;

namespace ray::renderer_core_api::resources
{
	u64 ResourcesLoader::sNumBuffers = 0;
	u64 ResourcesLoader::sNumTextures = 0;

	void ResourcesLoader::Initialize() noexcept
	{
		_begin = globals::gGMemManager.AllocateFromUploadBuffer(MAX_BUFFER_SIZE);
		_currentPointer = static_cast<u8*>(_begin);
		_end = _currentPointer + MAX_BUFFER_SIZE;
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
		ray_assert(_currentPointer + subresourceFootprint.RowPitch * subresourceFootprint.Height <= _end, "Out of upload buffer!")

		D3D12_PLACED_SUBRESOURCE_FOOTPRINT placedSubres;
		placedSubres.Footprint = subresourceFootprint;
		placedSubres.Offset = static_cast<u64>(_currentPointer - static_cast<u8*>(_begin));
		u64 offset = placedSubres.Offset;

		const void* textureData = static_cast<const void*>(texture.GetData().GetData());
		for (size_t i = 0; i < subresourceFootprint.Height; ++i)
		{
			u8* dest = static_cast<u8*>(_begin) + offset + i * subresourceFootprint.RowPitch;
			const u8* src = static_cast<const u8*>(textureData) + i * subresourceFootprint.Width;
			memcpy(dest, src, 16 * subresourceFootprint.Width);
		}

		u64 resourceId = TEXTURE_MASK | sNumTextures++;
		globals::gGlobalResourcesTable.AddResource(LoadedResource::eTexture, offset, resourceId, false);
	}

	void ResourcesLoader::SetDataToUploadBuffer(void* buffer, size_t bufferSize, size_t alignment) noexcept
	{
		_currentPointer = reinterpret_cast<u8*>(AlignUp(reinterpret_cast<size_t>(_currentPointer), alignment));
		ray_assert(_currentPointer + bufferSize <= _end, "Out of upload buffer!")

		auto offset = static_cast<u64>(_currentPointer - static_cast<u8*>(_begin));
		memcpy(_currentPointer, buffer, bufferSize);
		_currentPointer += bufferSize;

		u64 resourceId = BUFFER_MASK | sNumBuffers++;

		globals::gGlobalResourcesTable.AddResource(LoadedResource::eBuffer, offset, resourceId, false);
	}

}