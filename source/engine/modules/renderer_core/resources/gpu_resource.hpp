#include "resources.hpp"
#include "../renderer_globals.hpp"

#include <core/core.hpp>

namespace ray::renderer_core_api::resources
{
	class GpuResource
	{
		friend class CommandContext;
		friend class GraphicsContext;
		friend class ComputeContext;
	public:
		GpuResource() noexcept
			: _resource(gClassHelper->CreateResource())
			, _userAllocatedMemory(nullptr)
			, _gpuVirtualAddress(GPU_VIRTUAL_ADDRESS_NULL)
			, _usageState(ResourceState::eCommon)
			, _transitioningState(static_cast<ResourceState>(-1))
		{}

		GpuResource(IResource* inResource, ResourceState currentState) noexcept
			: _resource(inResource)
			, _usageState(currentState)
			, _gpuVirtualAddress(GPU_VIRTUAL_ADDRESS_NULL)
			, _transitioningState(static_cast<ResourceState>(-1))
			, _userAllocatedMemory(nullptr)
		{}

		void Destroy() noexcept
		{
			_resource = nullptr;
			_gpuVirtualAddress = GPU_VIRTUAL_ADDRESS_NULL;
			if (_userAllocatedMemory == nullptr)
			{
				// TODO: VirtualFree
				free(_userAllocatedMemory);
				_userAllocatedMemory = nullptr;
			}
		}

		GpuVirtualAddress GetGpuVirtualAddress() const noexcept { return _gpuVirtualAddress; }
		IResource* GetResource() const noexcept { return _resource; }
		void* GetUserAllocatedMemory() const noexcept { return _userAllocatedMemory; }


	protected:
		void* _userAllocatedMemory;
		GpuVirtualAddress _gpuVirtualAddress;
		IResource* _resource;
		ResourceState _usageState;
		ResourceState _transitioningState;

	};
}