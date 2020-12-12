#pragma once

#include <core/core.hpp>
#include <core/lib/array.hpp>
#include <core/debug/assert.hpp>

#include <map> // temporary. implement ray's map, please

#include <d3d12.h>

#define RESOURCE_ID_NULL static_cast<u64>(-1)

namespace ray::renderer_core_api::resources
{
	struct AllocatedResource;

	class ResourcesTable
	{
	public:
		const AllocatedResource& GetResourceByGlobalId(u64 globalId) const noexcept;
		const AllocatedResource& GetTextureById(u64 textureId) const noexcept;
		const AllocatedResource& GetBufferById(u64 bufferId) const noexcept;

		void SetTexture(const AllocatedResource& texture) noexcept;
		void SetBuffer(const AllocatedResource& buffer) noexcept;
		void SetResource(const AllocatedResource& resource, u64 localId, u16 resourceDimension) noexcept;

		u64 GetResourceGlobalId(u64 localId, u16 resourceDimension, bool bCheckExisting = true) const noexcept;
		u64 GetTexturesCount() const noexcept
		{
			return _texturesCount;
		}

		u64 GetBuffersCount() const noexcept
		{
			return _buffersCount;
		}

		u64 GetRTVsAndDSVsCount() const noexcept
		{
			return _rtvsAndDsvsCount;
		}

	private:
		std::map<u64, AllocatedResource> _table;
		u64 _texturesCount = 0;
		u64 _buffersCount = 0;
		u64 _rtvsAndDsvsCount = 0;
	
	};
}