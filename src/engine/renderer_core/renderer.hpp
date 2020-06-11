#pragma once
#include <core/object/object.hpp>
#include <app_framework/base/platform_window.hpp>

#include "ray_renderer_core_class_helper.hpp"


namespace ray::renderer_core_api
{
	
	struct IRenderer final : public Object
	{
		void Initialize(ray::core::IPlatformWindow* window);
		void Draw(); //temporary
		void UpdatePipeline(); //temporary
		void WaitForPreviousFrame(); //temporary
		void Shutdown();

		void SetRunning(bool running) { _running = running; }
		bool IsRunning() { return _running; }

private:
	bool _running;
	resources::IResourceBarrier* _resource_barrier;
	IRRCClassHelper* _class_helper;
	IDevice* _device;
	ICommandList* _command_list;
	IDescriptorHeap* _descriptor_heap;
	ISwapChain* _swap_chain;
	ICommandQueue* _command_queue;
	static const u32 FRAME_BUFFER_COUNT = 3;
	IFenceEvent* _fence_event;
	u32 _frame_index;
	resources::IResource* _render_targets[FRAME_BUFFER_COUNT];
	ICommandAllocator* _command_allocators[FRAME_BUFFER_COUNT];
	IFence* _fences[FRAME_BUFFER_COUNT];
	u64 _fence_values[FRAME_BUFFER_COUNT];

};

using GetRRCClassHelper_t = IRRCClassHelper* (*)();

}