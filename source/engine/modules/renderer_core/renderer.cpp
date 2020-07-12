#include "renderer.hpp"

//ñäåëàéòå óæå èíòåðôåéñ äëÿ ðàáîòû ñ ìîäóëÿìè
#include <Windows.h>

#include <engine/state/state.hpp>

namespace ray::renderer_core_api
{

	void IRenderer::Initialize(ray::core::IPlatformWindow* window, IModule* rendererModule)
	{
		_classHelper = static_cast<IRRCClassHelper*>(rendererModule->QueryModuleInterface());

		_device = _classHelper->CreateDevice();
		_rtvCommandList = _classHelper->CreateCommandList();
		_3dCommandQueue = _classHelper->CreateCommandQueue();
		_descriptorHeap = _classHelper->CreateDescriptorHeap();
		_swapChain = _classHelper->CreateSwapChain();
		_fenceEvent = _classHelper->CreateFenceEvent();
		_resourceBarrier = _classHelper->CreateResourceBarrier();

		for (u32 i = 0; i < FRAME_BUFFER_COUNT; i++)
		{
			_renderTargets[i] = _classHelper->CreateResource();
			_commandAllocators[i] = _classHelper->CreateCommandAllocator();
			_fences[i] = _classHelper->CreateFence();
		}

		if (!_device->Initialize())
			return;

		CommandQueueDesc cmdQueueDesc;
		if (!_device->CreateCommandQueue(cmdQueueDesc, _3dCommandQueue))
			return;

		SwapChainDesc swapChainDesc;
		swapChainDesc._back_buffer._width = window->GetWidth();
		swapChainDesc._back_buffer._height = window->GetHeight();
		swapChainDesc._buffer_count = FRAME_BUFFER_COUNT;
		swapChainDesc._command_queue = _3dCommandQueue;
		swapChainDesc._fullscreen = false;
		swapChainDesc._output_window = static_cast<HWND>(window->GetWindowHandleRaw());
		if (!_swapChain->Initialize(swapChainDesc, _frameIndex))
			return;

		DescriptorHeapDesc rtvHeapDesc;
		rtvHeapDesc._num_descriptors = FRAME_BUFFER_COUNT;
		rtvHeapDesc._shader_visible = false;
		rtvHeapDesc._type = DescriptorHeapType::eRTV;
		_device->CreateDescriptorHeap(rtvHeapDesc, _descriptorHeap);

		ICPUDescriptor* rtvDescriptor = _classHelper->CreateCPUDescriptor();
		rtvDescriptor->Initialize(_descriptorHeap);
		rtvDescriptor->SetDescriptorSize(_device->GetDescriptorHandleIncrementSize(DescriptorHeapType::eRTV));

		for (u32 i = 0; i < FRAME_BUFFER_COUNT; i++)
		{
			if (!_swapChain->GetBuffer(i, _renderTargets[i]))
				return;

			RenderTargetViewDesc rtvDesc = {};
			_device->CreateRenderTargetView(_renderTargets[i], rtvDesc, rtvDescriptor);

			if (!rtvDescriptor->Offset(1))
				return;

			if (!_device->CreateCommandAllocator(_commandAllocators[i], CommandListType::eDirect))
				return;

			_fenceValues[i] = 0;

			if (!_device->CreateFence(_fences[i], 0))
				return;
		}

		if (!_device->CreateCommandList(_rtvCommandList, _commandAllocators[0], nullptr, CommandListType::eDirect))
			return;

		_rtvCommandList->Close();

		Add3DCommandList(_rtvCommandList);

		if (!_device->CreateFenceEvent(_fenceEvent, nullptr, false, false))
			return;

		delete rtvDescriptor;
	}

	void IRenderer::BeginScene()
	{
		WaitForPreviousFrame();

		ICPUDescriptor* rtvHandle = _classHelper->CreateCPUDescriptor();
		rtvHandle->Initialize(_descriptorHeap);
		rtvHandle->SetDescriptorSize(_device->GetDescriptorHandleIncrementSize(DescriptorHeapType::eRTV));
		rtvHandle->Offset(_frameIndex);

		_commandAllocators[_frameIndex]->Reset();

		_rtvCommandList->Reset(_commandAllocators[_frameIndex], nullptr);

		_resourceBarrier->Transition(_renderTargets[_frameIndex], resources::ResourceState::ePresent, resources::ResourceState::eRenderTarget);
		_rtvCommandList->ResourceBarrier(_resourceBarrier, 1);

		_rtvCommandList->OMSetRenderTargetView(1, rtvHandle, nullptr, false);

		float clearColor[] = { 1.f, 0.f, 0.f, 1.f };
		_rtvCommandList->ClearRenderTarget(rtvHandle, clearColor);

		_resourceBarrier->Transition(_renderTargets[_frameIndex], resources::ResourceState::eRenderTarget, resources::ResourceState::ePresent);
		_rtvCommandList->ResourceBarrier(_resourceBarrier, 1);

		_rtvCommandList->Close();

		delete rtvHandle;
	} 

	void IRenderer::EndScene()
	{
		_swapChain->Present(0, 0);
	}

	void IRenderer::Execute()
	{
		_3dCommandQueue->SetCommandLists(_3dLists.data(), _3dLists.size());
		_3dCommandQueue->ExecuteCommandLists();
		_3dCommandQueue->Signal(_fences[_frameIndex], u32(_fenceValues[_frameIndex]));
	}


	void IRenderer::WaitForPreviousFrame()
	{
		_frameIndex = _swapChain->GetCurrentBackBufferIndex();
		if (_fences[_frameIndex]->GetCompletedValue() < _fenceValues[u64(_frameIndex)])
		{
			_fences[_frameIndex]->SetEventOnCompletion(_fenceEvent, _fenceValues[_frameIndex]);

			_fenceEvent->WaitFor();
		}
		_fenceValues[_frameIndex]++;
	}

	void IRenderer::Shutdown()
	{
		for (u32 i = 0; i < FRAME_BUFFER_COUNT; i++)
		{
			_frameIndex = i;
			// WaitForPreviousFrame();
		}

		delete _rtvCommandList;
		delete _3dCommandQueue;
		delete _descriptorHeap;
		delete _device;
		for (size_t i = 0; i < FRAME_BUFFER_COUNT; i++)
		{
			delete _commandAllocators[i];
			delete _fences[i];
			delete _renderTargets[i];
		}
		delete _fenceEvent;
		delete _resourceBarrier;
		delete _swapChain;
		// delete _class_helper;
	}
}
