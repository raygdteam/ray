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
		_command_queue = _class_helper->CreateCommandQueue();
		_descriptor_heap = _class_helper->CreateDescriptorHeap();
		_swap_chain = _class_helper->CreateSwapChain();
		_fence_event = _class_helper->CreateFenceEvent();
		_resource_barrier = _class_helper->CreateResourceBarrier();

		for (u32 i = 0; i < FRAME_BUFFER_COUNT; i++)
		{
			_render_targets[i] = _class_helper->CreateResource();
			_command_allocators[i] = _class_helper->CreateCommandAllocator();
			_fences[i] = _class_helper->CreateFence();
		}

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
		if (!_swap_chain->Initialize(swapChainDesc, _frame_index))
			return;

		DescriptorHeapDesc rtvHeapDesc;
		rtvHeapDesc._num_descriptors = FRAME_BUFFER_COUNT;
		rtvHeapDesc._shader_visible = false;
		rtvHeapDesc._type = DescriptorHeapType::descriptor_heap_type_rtv;
		_device->CreateDescriptorHeap(rtvHeapDesc, _descriptor_heap);

		ICPUDescriptor* rtvDescriptor = _class_helper->CreateCPUDescriptor();
		rtvDescriptor->Initialize(_descriptor_heap);
		rtvDescriptor->SetDescriptorSize(_device->GetDescriptorHandleIncrementSize(DescriptorHeapType::descriptor_heap_type_rtv));

		for (u32 i = 0; i < FRAME_BUFFER_COUNT; i++)
		{
			if(!_swap_chain->GetBuffer(i, _render_targets[i]))
				return;

			RenderTargetViewDesc rtvDesc = {};
			_device->CreateRenderTargetView(_render_targets[i], rtvDesc, rtvDescriptor);

			if (!rtvDescriptor->Offset(1))
				return;
		
			if (!_device->CreateCommandAllocator(_command_allocators[i], CommandListType::direct))
				return;

			_fence_values[i] = 0;

			if (!_device->CreateFence(_fences[i], 0))
				return;
		}

		if (!_device->CreateCommandList(_command_list, _command_allocators[0], nullptr, CommandListType::direct))
			return;

		_command_list->Close();

		if (!_device->CreateFenceEvent(_fence_event, nullptr, false, false))
			return;

	}

	void IRenderer::UpdatePipeline()
	{
		bool result;
		WaitForPreviousFrame();

		ICPUDescriptor* rtvHandle = _class_helper->CreateCPUDescriptor();
		rtvHandle->Initialize(_descriptor_heap);
		rtvHandle->SetDescriptorSize(_device->GetDescriptorHandleIncrementSize(DescriptorHeapType::descriptor_heap_type_rtv));
		rtvHandle->Offset(_frame_index);

		result = _command_allocators[_frame_index]->Reset();
		SetRunning(result);

		result = _command_list->Reset(_command_allocators[_frame_index], nullptr);
		SetRunning(result);

		_resource_barrier->Transition(_render_targets[_frame_index], resources::ResourceState::present, resources::ResourceState::render_target);
		_command_list->ResourceBarrier(_resource_barrier, 1);

		_command_list->OMSetRenderTargetView(1, rtvHandle, nullptr, false);

		float clearColor[] = { 1.f, 0.f, 0.f, 1.f };
		_command_list->ClearRenderTarget(rtvHandle, clearColor);

		_resource_barrier->Transition(_render_targets[_frame_index], resources::ResourceState::render_target, resources::ResourceState::present);
		_command_list->ResourceBarrier(_resource_barrier, 1);

	 	result = _command_list->Close();
		SetRunning(result);
	}

	void IRenderer::Draw()
	{
		bool result;
		UpdatePipeline();

		ICommandList* lists[] = { _command_list };
		_command_queue->SetCommandLists(lists, 1);
		_command_queue->ExecuteCommandLists();

		result = _command_queue->Signal(_fences[_frame_index], _fence_values[_frame_index]);
		SetRunning(result);

		result = _swap_chain->Present(0, 0);
		SetRunning(result);
	}

	void IRenderer::WaitForPreviousFrame()
	{
		bool result;
		_frame_index = _swap_chain->GetCurrentBackBufferIndex();
		if (_fences[_frame_index]->GetCompletedValue() < _fence_values[_frame_index])
		{
			result = _fences[_frame_index]->SetEventOnCompletion(_fence_event, _fence_values[_frame_index]);
			SetRunning(result);

			_fence_event->WaitFor();
		}
		_fence_values[_frame_index]++;
	}

	void IRenderer::Shutdown()
	{
		for (size_t i = 0; i < FRAME_BUFFER_COUNT; i++)
		{
			_frame_index = i;
			WaitForPreviousFrame();
		}

		delete _command_queue;
		delete _command_list;
		delete _descriptor_heap;
		delete _device;
		for (size_t i = 0; i < FRAME_BUFFER_COUNT; i++)
		{
			delete _command_allocators[i];
			delete _fences[i];
			delete _render_targets[i];
		}
		delete _fence_event;
		delete _resource_barrier;
		delete _swap_chain;
		delete _class_helper;
	}
}