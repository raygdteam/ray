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

		_device = getDevice();
		_command_list = getCommandList();
		_command_allocator = getCommandAllocator();
		_command_queue = getCommandQueue();
		_fence = getFence();
		_descriptor_heap = getDescriptorHeap();
		_swap_chain = getSwapChain();

		if (!_device->Initialize())
			return;

		CommandQueueDesc cmdQueueDesc;
		if (!_device->CreateCommandQueue(cmdQueueDesc, _command_queue))
			return;


	}
}