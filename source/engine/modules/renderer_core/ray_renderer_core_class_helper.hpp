#pragma once

#include <core/module/module_meta.hpp>
#include "device.hpp"
#include "swap_chain.hpp"
#include "resources/resources.hpp"
#include "resources/index_buffer.hpp"
#include "resources/vertex_buffer.hpp"
#include "vertex_shader.hpp"
#include "pixel_shader.hpp"

namespace ray::renderer_core_api
{
	class IRRCClassHelper : public IModuleInterface
	{
	public:
		virtual ICommandAllocator* CreateCommandAllocator() = 0;
		virtual ICommandList* CreateCommandList() = 0;
		virtual ICommandQueue* CreateCommandQueue() = 0;
		virtual IDescriptorHeap* CreateDescriptorHeap() = 0;
		virtual IFence* CreateFence() = 0;
		virtual IDevice* CreateDevice() = 0;
		virtual ISwapChain* CreateSwapChain() = 0;
		virtual resources::IResource* CreateResource() = 0;
		virtual ICPUDescriptor* CreateCPUDescriptor() = 0;
		virtual IGPUDescriptor* CreateGPUDescriptor() = 0;
		virtual IFenceEvent* CreateFenceEvent() = 0;
		virtual resources::IResourceBarrier* CreateResourceBarrier() = 0;
		virtual IRootSignature* CreateRootSignature() = 0;
		virtual IVertexShader* CreateVertexShader() = 0;
		virtual IPixelShader* CreatePixelShader() = 0;
		virtual resources::IVertexBuffer* CreateVertexBuffer() = 0;
		virtual resources::IIndexBuffer* CreateIndexBuffer() = 0;

	};
}