#pragma once
#include <core/object/object.hpp>
#include <app_framework/base/platform_window.hpp>

#include "device.hpp"
#include "swap_chain.hpp"
#include "resources/resources.hpp"


namespace ray::renderer_core_api
{
	
struct IRenderer : public Object
{
	virtual void Initialize(ray::core::IPlatformWindow* window);
	//virtual void Draw();
	virtual void Shutdown();

private:
	IDevice* _device;
	ICommandAllocator* _command_allocator;
	ICommandList* _command_list;
	IDescriptorHeap* _descriptor_heap;
	IFence* _fence;
	ISwapChain* _swap_chain;
	ICommandQueue* _command_queue;
	static const u32 FRAME_BUFFER_COUNT = 3;
	s32 _rtv_descriptor_size;
	resources::IResource* _render_targets[FRAME_BUFFER_COUNT];

};

using GetRendererDevice_t = IDevice* (*)();
using GetRendererCommandList_t = ICommandList* (*)();
using GetRendererCommandAllocator_t = ICommandAllocator* (*)();
using GetRendererCommandQueue_t = ICommandQueue* (*)();
using GetRendererDescriptorHeap_t = IDescriptorHeap* (*)();
using GetRendererFence_t = IFence* (*)();
using GetRendererSwapChain_t = ISwapChain* (*)();
using GetRendererResource_t = resources::IResource* (*)();

}