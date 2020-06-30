#include "pch.hpp"
#include <renderer_core/ray_renderer_core_class_helper.hpp>
#include "d3d12_device.hpp"
#include "d3d12_swap_chain.hpp"
#include "d3d12_resources.hpp"
#include "d3d12_command_allocator.hpp"
#include "d3d12_command_list.hpp"
#include "d3d12_command_queue.hpp"
#include "d3d12_descriptor_heap.hpp"
#include "d3d12_fence.hpp"

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

		IResourceBarrier* CreateResourceBarrier() override
		{
			return new resources::D3D12ResourceBarrier;
		}

	};
}

extern "C" __declspec(dllexport)
IRRCClassHelper* GetRRCClassHelper()
{
	return new ray::renderer::d3d12::D3D12RRCClassHelper;
}

// Пусть посидит тут временно.

#include "mimalloc/mimalloc.h"

void operator delete(void* p) noexcept { free(p); };
void operator delete[](void* p) noexcept { free(p); };

void* operator new(std::size_t n) noexcept(false) { return mi_new(n); }
void* operator new[](std::size_t n) noexcept(false) { return mi_new(n); }

void* operator new  (std::size_t n, const std::nothrow_t& tag) noexcept { (void)(tag); return mi_new_nothrow(n); }
void* operator new[](std::size_t n, const std::nothrow_t& tag) noexcept { (void)(tag); return mi_new_nothrow(n); }

#if (__cplusplus >= 201402L || _MSC_VER >= 1916)
void operator delete  (void* p, std::size_t n) noexcept { mi_free_size(p, n); };
void operator delete[](void* p, std::size_t n) noexcept { mi_free_size(p, n); };
#endif

#if (__cplusplus > 201402L || defined(__cpp_aligned_new))
void operator delete  (void* p, std::align_val_t al) noexcept { mi_free_aligned(p, static_cast<size_t>(al)); }
void operator delete[](void* p, std::align_val_t al) noexcept { mi_free_aligned(p, static_cast<size_t>(al)); }
void operator delete  (void* p, std::size_t n, std::align_val_t al) noexcept { mi_free_size_aligned(p, n, static_cast<size_t>(al)); };
void operator delete[](void* p, std::size_t n, std::align_val_t al) noexcept { mi_free_size_aligned(p, n, static_cast<size_t>(al)); };

void* operator new(std::size_t n, std::align_val_t al)   noexcept(false) { return mi_new_aligned(n, static_cast<size_t>(al)); }
void* operator new[](std::size_t n, std::align_val_t al) noexcept(false) { return mi_new_aligned(n, static_cast<size_t>(al)); }
void* operator new  (std::size_t n, std::align_val_t al, const std::nothrow_t&) noexcept { return mi_new_aligned_nothrow(n, static_cast<size_t>(al)); }
void* operator new[](std::size_t n, std::align_val_t al, const std::nothrow_t&) noexcept { return mi_new_aligned_nothrow(n, static_cast<size_t>(al)); }
#endif
