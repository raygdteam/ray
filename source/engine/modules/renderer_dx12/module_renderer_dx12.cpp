#include "pch.hpp"
#include <core/core.hpp>
#include <core/memory/new_delete_override.hpp>
#include <core/module/module.hpp>

#include <renderer_core/ray_renderer_core_class_helper.hpp>
#include "d3d12_device.hpp"
#include "d3d12_swap_chain.hpp"
#include "d3d12_resources.hpp"
#include "d3d12_command_allocator.hpp"
#include "d3d12_command_list.hpp"
#include "d3d12_command_queue.hpp"
#include "d3d12_descriptor_heap.hpp"
#include "d3d12_fence.hpp"
#include "d3d12_root_signature.hpp"
#include "d3d12_vertex_shader.hpp"
#include "d3d12_pixel_shader.hpp"
#include "d3d12_vertex_buffer.hpp"
#include "d3d12_index_buffer.hpp"

using namespace ray::renderer::d3d12;

class D3D12ModuleInterface : public IRRCClassHelper
{
public:
	IDevice* CreateDevice()
	{
		return new D3D12Device;
	}

	ISwapChain* CreateSwapChain()
	{
		return new D3D12SwapChain;
	}

	IResource* CreateResource()
	{
		return new ::ray::renderer::d3d12::resources::D3D12Resource;
	}

	ICommandAllocator* CreateCommandAllocator()
	{
		return new D3D12CommandAllocator;
	}

	ICommandList* CreateCommandList()
	{
		return new D3D12CommandList;
	}

	ICommandQueue* CreateCommandQueue()
	{
		return new D3D12CommandQueue;
	}

	IDescriptorHeap* CreateDescriptorHeap()
	{
		return new D3D12DescriptorHeap;
	}

	IFence* CreateFence()
	{
		return new D3D12Fence;
	}

	ICPUDescriptor* CreateCPUDescriptor()
	{
		return new D3D12CPUDescriptor;
	}

	IGPUDescriptor* CreateGPUDescriptor()
	{
		return new D3D12GPUDescriptor;
	}

	IFenceEvent* CreateFenceEvent()
	{
		return new D3D12FenceEvent;
	}

	IResourceBarrier* CreateResourceBarrier()
	{
		return new ::ray::renderer::d3d12::resources::D3D12ResourceBarrier;
	}

	IRootSignature* CreateRootSignature()
	{
		return new D3D12RootSignature;
	}

	IVertexShader* CreateVertexShader()
	{
		return new D3D12VertexShader;
	}

	IPixelShader* CreatePixelShader()
	{
		return new D3D12PixelShader;
	}

	IVertexBuffer* CreateVertexBuffer()
	{
		return new ::ray::renderer::d3d12::resources::D3D12VertexBuffer;
	}

	IIndexBuffer* CreateIndexBuffer()
	{
		return new ::ray::renderer::d3d12::resources::D3D12IndexBuffer;
	}
};

struct Renderer_Dx12_Module : public IModule
{
	void OnLoad() override
	{
		this->Meta = ModuleMeta { "renderer_dx12", false };
	}
	
	void OnUnload() override
	{
		
	}
	
	IModuleInterface* QueryModuleInterface() override
	{
		static D3D12ModuleInterface interface_;
		return &interface_;
	}
	
	~Renderer_Dx12_Module() override
	{
		
	}
};

REGISTER_MODULE(Renderer_Dx12_Module);

#ifdef RAY_RELEASE
int foo(const int bar)
{
	init_res = bar >> 1;
	return init_res;
}
#endif
