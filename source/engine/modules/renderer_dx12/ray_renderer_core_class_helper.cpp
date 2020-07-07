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
#include "d3d12_root_signature.hpp"
#include "d3d12_vertex_shader.hpp"
#include "d3d12_pixel_shader.hpp"
#include "d3d12_vertex_buffer.hpp"
#include "d3d12_index_buffer.hpp"

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

		IRootSignature* CreateRootSignature() override
		{
			return new D3D12RootSignature;
		}

		IVertexShader* CreateVertexShader() override
		{
			return new D3D12VertexShader;
		}

		IPixelShader* CreatePixelShader() override
		{
			return new D3D12PixelShader;
		}

		IVertexBuffer* CreateVertexBuffer() override
		{
			return new resources::D3D12VertexBuffer;
		}

		IIndexBuffer* CreateIndexBuffer() override
		{
			return new resources::D3D12IndexBuffer;
		}

	};
}

/*extern "C" __declspec(dllexport)
IRRCClassHelper* GetRRCClassHelper()
{
	return new ray::renderer::d3d12::D3D12RRCClassHelper;
}*/
