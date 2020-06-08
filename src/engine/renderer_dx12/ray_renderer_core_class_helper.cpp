#include <renderer_core/ray_renderer_core_class_helper.hpp>
#include "device.hpp"
#include "swap_chain.hpp"
#include "resources.hpp"
#include "command_allocator.hpp"
#include "command_list.hpp"
#include "command_queue.hpp"
#include "descriptor_heap.hpp"
#include "fence.hpp"

using namespace ray::renderer_core_api;

namespace ray::renderer::d3d12
{
	class D3D12RRCClassHelper : public IRRCClassHelper
	{
	public:
		IDevice* CreateDevice() override 
		{
			return new D3D12Device;
		}

		ISwapChain* CreateSwapChain() override 
		{
			return new D3D12SwapChain;
		}

		IResource* CreateResource() override 
		{
			return new resources::D3D12Resource;
		}

		ICommandAllocator* CreateCommandAllocator() override 
		{
			return new D3D12CommandAllocator;
		}

		ICommandList* CreateCommandList() override 
		{
			return new D3D12CommandList;
		}
		
		ICommandQueue* CreateCommandQueue() override 
		{
			return new D3D12CommandQueue;
		}

		IDescriptorHeap* CreateDescriptorHeap() override 
		{
			return new D3D12DescriptorHeap;
		}

		IFence* CreateFence() override 
		{
			return new D3D12Fence;
		}

		ICPUDescriptor* CreateCPUDescriptor() override
		{
			return new D3D12CPUDescriptor;
		}

		IGPUDescriptor* CreateGPUDescriptor() override
		{
			return new D3D12GPUDescriptor;
		}

		IFenceEvent* CreateFenceEvent()
		{
			return new D3D12FenceEvent;
		}

	};
}

extern "C" __declspec(dllexport)
IRRCClassHelper* GetRRCClassHelper()
{
	return new ray::renderer::d3d12::D3D12RRCClassHelper;
}