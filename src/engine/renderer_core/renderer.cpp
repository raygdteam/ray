#include "renderer.hpp"

//сделайте уже интерфейс для работы с модулями
#include <ray/os/include.hpp>

namespace ray::renderer_core_api
{

	void IRenderer::Initialize(ray::core::IPlatformWindow* window)
	{
		HMODULE hModule = LoadLibrary("renderer_dx12");
		GetRendererDevice_t getDevice = (GetRendererDevice_t)GetProcAddress(hModule, "GetRendererDevice");
		GetRendererCommandList_t getCommandList = (GetRendererCommandList_t)GetProcAddress(hModule, "GetRendererCommandList");
		GetRendererCommandAllocator_t getCommandAllocator = (GetRendererCommandAllocator_t)GetProcAddress(hModule, "GetRendererCommandAllocator");
		GetRendererCommandQueue_t getCommandQueue = (GetRendererCommandQueue_t)GetProcAddress(hModule, "GetRendererCommandQueue");
		GetRendererFence_t getFence = (GetRendererFence_t)GetProcAddress(hModule, "GetRendererFence");
		GetRendererDescriptorHeap_t getDescriptorHeap = (GetRendererDescriptorHeap_t)GetProcAddress(hModule, "GetRendererDescriptorHeap");
		GetRendererSwapChain_t getSwapChain = (GetRendererSwapChain_t)GetProcAddress(hModule, "GetRendererSwapChain");
		GetRendererResource_t getResource = (GetRendererResource_t)GetProcAddress(hModule, "GetRendererResource");

		_device = getDevice();
		_command_list = getCommandList();
		_command_allocator = getCommandAllocator();
		_command_queue = getCommandQueue();
		_fence = getFence();
		_descriptor_heap = getDescriptorHeap();
		_swap_chain = getSwapChain();
		for (u32 i = 0; i < FRAME_BUFFER_COUNT; i++)
			_render_targets[i] = getResource();

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