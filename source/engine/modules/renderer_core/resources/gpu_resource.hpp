#pragma once
#include <d3d12.h>

#include <core/core.hpp>

namespace ray::renderer_core_api
{
	class CommandContext;
	class ComputeContext;
	class GraphicsContext;
}

namespace ray::renderer_core_api::resources 
{
	enum class ResourceMappingMode
	{
		eReadAccess,
		eWriteAccess,
	};

	class GpuResource
	{
		friend class ray::renderer_core_api::CommandContext;
		friend class ::ray::renderer_core_api::GraphicsContext;
		friend class ::ray::renderer_core_api::ComputeContext;

	protected:
		ID3D12Resource* _resource;
		D3D12_RESOURCE_STATES _usageState;
		D3D12_RESOURCE_STATES _transitioningState;
		u64 _resourceSize;

	public:
		GpuResource() noexcept
			: _resource(nullptr)
			, _usageState(D3D12_RESOURCE_STATE_COMMON)
			, _transitioningState(static_cast<D3D12_RESOURCE_STATES>(-1))
		{}

		GpuResource(ID3D12Resource* inResource, D3D12_RESOURCE_STATES currentState) noexcept
			: _resource(inResource)
			, _usageState(currentState)
			, _transitioningState(static_cast<D3D12_RESOURCE_STATES>(-1))
		{}

		virtual ~GpuResource() 
		{
			Destroy();
		}

		void Destroy() noexcept
		{
			if (_resourceSize != 0)
			{
				Release();
				_resourceSize = 0;
			}
		}

		virtual void Release() noexcept {}

		ID3D12Resource* GetResource() const noexcept { return _resource; }
		u64 GetResourceSize() const noexcept { return _resourceSize; }

		void Map(ResourceMappingMode mode, void** data) const noexcept
		{
			D3D12_RANGE range;
			range.Begin = 0;
			range.End = 0;

			D3D12_RANGE* access = &range;

			if (mode == ResourceMappingMode::eReadAccess)
				access = nullptr;

			_resource->Map(0, access, data);
		}

		void Unmap() const noexcept
		{
			_resource->Unmap(0, nullptr);
		}

	};

}
