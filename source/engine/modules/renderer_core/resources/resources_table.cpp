#include "resources_table.hpp"

namespace ray::renderer_core_api::resources
{
	void ResourcesTable::AddResource(LoadedResource::Type type, u64 offset, u64 resourceId, bool bFlushed) noexcept
	{
		LoadedResource resource;
		resource.bFlushed = bFlushed;
		resource.CpuSide.Offset = offset;
		resource.ResourceType = type;

		_table.emplace(std::make_pair(resourceId, resource));
	}
}