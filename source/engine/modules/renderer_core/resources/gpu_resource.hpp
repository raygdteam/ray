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
	class GpuResource
	{
		friend class ray::renderer_core_api::CommandContext;
		friend class ::ray::renderer_core_api::GraphicsContext;
		friend class ::ray::renderer_core_api::ComputeContext;
	public:
		GpuResource() noexcept
			: _userAllocatedMemory(nullptr)
			, _gpuVirtualAddress(0)
			, _resource(nullptr)
			, _usageState(D3D12_RESOURCE_STATE_COMMON)
			, _transitioningState(static_cast<D3D12_RESOURCE_STATES>(-1))
		{}

		GpuResource(ID3D12Resource* inResource, D3D12_RESOURCE_STATES currentState) noexcept
			: _userAllocatedMemory(nullptr)
			, _gpuVirtualAddress(0)
			, _resource(inResource)
			, _usageState(currentState)
			, _transitioningState(static_cast<D3D12_RESOURCE_STATES>(-1))
		{}

		virtual void Destroy() noexcept
		{
			_resource = nullptr;
			_gpuVirtualAddress = 0;
			if (_userAllocatedMemory == nullptr)
			{
				// TODO: VirtualFree
				VirtualFree(_userAllocatedMemory, 0, MEM_RELEASE);
				_userAllocatedMemory = nullptr;
			}
		}

		D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress() const noexcept { return _gpuVirtualAddress; }
		ID3D12Resource* GetResource() const noexcept { return _resource; }
		void* GetUserAllocatedMemory() const noexcept { return _userAllocatedMemory; }


	protected:
		void* _userAllocatedMemory;
		D3D12_GPU_VIRTUAL_ADDRESS _gpuVirtualAddress;
		ID3D12Resource* _resource;
		D3D12_RESOURCE_STATES _usageState;
		D3D12_RESOURCE_STATES _transitioningState;

	};
}
