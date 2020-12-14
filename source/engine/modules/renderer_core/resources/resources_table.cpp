#include "resources_table.hpp"
#include "graphics_memory_manager.hpp"
#include "gpu_allocator.hpp"

namespace ray::renderer_core_api::resources
{
	u64 ResourcesTable::GetResourceGlobalId(u64 localId, u16 resourceDimension, bool bCheckExisting) const noexcept
	{
		check(resourceDimension == RESOURCE_DIMENSION_BUFFER || resourceDimension == RESOURCE_DIMENSION_TEXTURE
			|| resourceDimension == RESOURCE_DIMENSION_RTV_AND_DSV)

		u64 globalId = (static_cast<u64>(resourceDimension) << RESOURCE_DIMENSION_SHIFT) | localId;

		return bCheckExisting && _table.find(globalId) == _table.end() ? RESOURCE_ID_NULL : globalId;
	}

	AllocatedResource* ResourcesTable::GetResourceByGlobalId(u64 globalId) const noexcept
	{
		return _table.at(globalId);
	}

	AllocatedResource* ResourcesTable::GetTextureById(u64 textureId) const noexcept
	{
		return GetResourceByGlobalId(GetResourceGlobalId(textureId, RESOURCE_DIMENSION_TEXTURE));
	}

	AllocatedResource* ResourcesTable::GetBufferById(u64 bufferId) const noexcept
	{
		return GetResourceByGlobalId(GetResourceGlobalId(bufferId, RESOURCE_DIMENSION_BUFFER));
	}

	void ResourcesTable::SetBuffer(const AllocatedResource& buffer) noexcept
	{
		SetResource(buffer, _buffersCount++, RESOURCE_DIMENSION_BUFFER);
	}

	void ResourcesTable::SetTexture(const AllocatedResource& texture) noexcept
	{
		SetResource(texture, _texturesCount++, RESOURCE_DIMENSION_TEXTURE);
	}

	void ResourcesTable::SetResource(const AllocatedResource& resource, u64 localId, u16 resourceDimension) noexcept
	{
		auto dest = AddResource(localId, resourceDimension);
		dest->Heap = resource.Heap;
		dest->Offset = resource.Offset;
		dest->Resource = resource.Resource;
	}

	AllocatedResource* ResourcesTable::AddResource(u64 localId, u16 resourceDimension) noexcept
	{
		AllocatedResource* resource = new AllocatedResource;
		_table.emplace(GetResourceGlobalId(localId, resourceDimension, false), resource);

		return resource;
	}

	AllocatedResource* ResourcesTable::AddTexture(u64 localId) noexcept
	{
		return AddResource(localId, RESOURCE_DIMENSION_TEXTURE);
	}

	AllocatedResource* ResourcesTable::AddBuffer(u64 localId) noexcept
	{
		return AddResource(localId, RESOURCE_DIMENSION_BUFFER);
	}
}