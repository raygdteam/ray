#pragma once
#include <core/object/object.hpp>
#include <app_framework/base/platform_window.hpp>

#include "ray_renderer_core_class_helper.hpp"
#include "resources/resources.hpp"


namespace ray::renderer_core_api
{
	
struct IRenderer final : public Object 
{
	void Initialize(ray::core::IPlatformWindow* window);
	void Shutdown();

private:
	IRRCClassHelper* _class_helper;
	IDevice* _device;
	ICommandList* _command_list;
	IDescriptorHeap* _descriptor_heap;
	IFence* _fence;
	ISwapChain* _swap_chain;
	ICommandQueue* _command_queue;
	static const u32 FRAME_BUFFER_COUNT = 3;
	ICPUDescriptor* _rtv_descriptor;
	resources::IResource* _render_targets[FRAME_BUFFER_COUNT];
	ICommandAllocator* _command_allocator[FRAME_BUFFER_COUNT];

};

using GetRRCClassHelper_t = IRRCClassHelper* (*)();

}