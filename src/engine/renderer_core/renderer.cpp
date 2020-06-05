#include "renderer.hpp"

//сделайте уже интерфейс для работы с модулями
#include <ray/os/include.hpp>

namespace ray::renderer_core_api
{

	void IRenderer::Initialize(ray::core::IPlatformWindow* window)
	{
		HMODULE hModule = LoadLibrary("renderer_dx12");
		GetRRCClassHelper_t getClassHelper = reinterpret_cast<GetRRCClassHelper_t>(GetProcAddress(hModule, "GetRRCClassHelper"));

		_class_helper = getClassHelper();

		_device = _class_helper->CreateDevice();
		_command_list = _class_helper->CreateCommandList();
		_command_allocator = _class_helper->CreateCommandAllocator();
		_command_queue = _class_helper->CreateCommandQueue();
		_fence = _class_helper->CreateFence();
		_descriptor_heap = _class_helper->CreateDescriptorHeap();
		_swap_chain = _class_helper->CreateSwapChain();
		for (u32 i = 0; i < FRAME_BUFFER_COUNT; i++)
			_render_targets[i] = _class_helper->CreateResource();

		if (!_device->Initialize())
			return;

		CommandQueueDesc cmdQueueDesc;
		if (!_device->CreateCommandQueue(cmdQueueDesc, _command_queue))
			return;

		SwapChainDesc swapChainDesc;
		swapChainDesc._back_buffer._width = window->GetWidth();
		swapChainDesc._back_buffer._height = window->GetHeight();
		swapChainDesc._buffer_count = FRAME_BUFFER_COUNT;
		swapChainDesc._command_queue = _command_queue;
		swapChainDesc._fullscreen = false;
		swapChainDesc._output_window = static_cast<HWND>(window->GetWindowHandleRaw());
		u32 frameIndex;
		if (!_swap_chain->Initialize(swapChainDesc, frameIndex))
			return;

		DescriptorHeapDesc rtvHeapDesc;
		rtvHeapDesc._num_descriptors = FRAME_BUFFER_COUNT;
		rtvHeapDesc._shader_visible = false;
		rtvHeapDesc._type = DescriptorHeapType::descriptor_heap_type_rtv;
		_device->CreateDescriptorHeap(rtvHeapDesc, _descriptor_heap);

		_rtv_descriptor_size = _device->GetDescriptorHandleIncrementSize(DescriptorHeapType::descriptor_heap_type_rtv);

		for (u32 i = 0; i < FRAME_BUFFER_COUNT; i++)
		{
			if(!_swap_chain->GetBuffer(i, _render_targets[i]))
				return;


		}
	}
}