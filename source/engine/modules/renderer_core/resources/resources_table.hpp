#pragma once

#include <core/core.hpp>
#include <core/lib/array.hpp>
#include <core/debug/assert.hpp>

#include <map> // temporary. implement ray's map, please

#include <d3d12.h>

// 0000 0000	0000 0000	0000 0000	0000 0000	0000 0000	0000 0000	0000 0000	0000 0000
// f000 0000 0000 0000
// 1000 0000 0000 0000 - buffer mask
// 2000 0000 0000 0000 - texture mask

namespace ray::renderer_core_api::resources
{
	struct LoadedResource
	{
		enum Type : u16
		{
			eBuffer = BIT(0),
			eTexture = BIT(1)
		};

		Type ResourceType;
		bool bFlushed;

		/*
		** a resource may have different offsets in gpu and cpu memory.
		** there is no need to use cpu side's offset if the resource got flushed.
		** if you need the cpu side's offset it is added to beginning of upload buffer.
		** otherwise it is added to beginning of texture or buffer pool depending on what is the resource type.
		*/
		union
		{
			struct
			{
				u64 Offset;
			} CpuSide;

			struct
			{
				u64 Offset;
			} GpuSide;
		};
	};

	class ResourcesTable
	{
	public:
		void AddResource(LoadedResource::Type type, u64 offset, u64 resourceId, bool bFlushed) noexcept;

		LoadedResource& GetResourceById(size_t resourceId) noexcept
		{
			return _table[resourceId];
		}

	private:
		std::map<u64, LoadedResource> _table;
		//Array<LoadedResource> _table;

	};
}