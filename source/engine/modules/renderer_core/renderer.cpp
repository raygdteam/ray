#include "renderer.hpp"
#include "renderer_globals.hpp"

//ñäåëàéòå óæå èíòåðôåéñ äëÿ ðàáîòû ñ ìîäóëÿìè
#include <Windows.h>

#include <engine/state/state.hpp>

bool IRenderer::_sbReady = false;

namespace ray::renderer_core_api
{

	void IRenderer::Initialize(ray::core::IPlatformWindow* window, IModule* rendererModule)
	{
		if (IsReady())
			return;

		gClassHelper = static_cast<IRRCClassHelper*>(rendererModule->QueryModuleInterface());

		gDevice = gClassHelper->CreateDevice();
		_rtvCommandList = gClassHelper->CreateCommandList();
		_3dCommandQueue = gClassHelper->CreateCommandQueue();
		_descriptorHeap = gClassHelper->CreateDescriptorHeap();
		_swapChain = gClassHelper->CreateSwapChain();
		_fenceEvent = gClassHelper->CreateFenceEvent();
		_resourceBarrier = gClassHelper->CreateResourceBarrier();

		for (u32 i = 0; i < FRAME_BUFFER_COUNT; i++)
		{
			_renderTargets[i] = gClassHelper->CreateResource();
			_commandAllocators[i] = gClassHelper->CreateCommandAllocator();
			_fences[i] = gClassHelper->CreateFence();
		}

		if (!gDevice->Initialize())
			return;

		CommandQueueDesc cmdQueueDesc;
		if (!gDevice->CreateCommandQueue(cmdQueueDesc, _3dCommandQueue))
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
		rtvHeapDesc.NumDescriptors = FRAME_BUFFER_COUNT;
		rtvHeapDesc.ShaderVisible = false;
		rtvHeapDesc.Type = DescriptorHeapType::eRTV;
		gDevice->CreateDescriptorHeap(rtvHeapDesc, _descriptorHeap);

		auto rtvDescriptor = gDescriptorAllocator->Allocate(3);

		for (u32 i = 0; i < FRAME_BUFFER_COUNT; i++)
		{
			if (!_swapChain->GetBuffer(i, _renderTargets[i]))
				return;

			RenderTargetViewDesc rtvDesc = {};
			gDevice->CreateRenderTargetView(_renderTargets[i], rtvDesc, rtvDescriptor);

			if (!rtvDescriptor->Increment())
				return;

			if (!gDevice->CreateCommandAllocator(_commandAllocators[i], CommandListType::eDirect))
				return;


			_fenceValues[i] = 0;

			if (!gDevice->CreateFence(_fences[i], 0))
				return;
		}

		if (!gDevice->CreateCommandList(_rtvCommandList, _commandAllocators[0], nullptr, CommandListType::eDirect))
			return;

		_rtvCommandList->Close();

		if (!gDevice->CreateFenceEvent(_fenceEvent, nullptr, false, false))
			return;

		delete rtvDescriptor;

		_sbReady = true;
	}

	void IRenderer::BeginScene()
	{
		WaitForPreviousFrame();

		ICPUDescriptor* rtvHandle = gClassHelper->CreateCPUDescriptor();
		_descriptorHeap->GetCPUDescriptorHandleForHeapStart(rtvHandle);
		rtvHandle->SetDescriptorSize(gDevice->GetDescriptorHandleIncrementSize(DescriptorHeapType::eRTV));
		rtvHandle->Offset(_frameIndex);

		_commandAllocators[_frameIndex]->Reset();

		_rtvCommandList->Reset(_commandAllocators[_frameIndex], nullptr);

		_resourceBarrier->Transition(_renderTargets[_frameIndex], resources::ResourceState::ePresent, resources::ResourceState::eRenderTarget);
		_rtvCommandList->ResourceBarrier(_resourceBarrier, 1);

		_rtvCommandList->OMSetRenderTargetView(1, rtvHandle, nullptr, false);

		float clearColor[] = { 1.f, 0.f, 0.f, 1.f };
		_rtvCommandList->ClearRenderTarget(rtvHandle, clearColor);
		
		delete rtvHandle;
	} 

	void IRenderer::EndScene()
	{
		_swapChain->Present(0, 0);
	}

	void IRenderer::Execute()
	{
		_resourceBarrier->Transition(_renderTargets[_frameIndex], resources::ResourceState::eRenderTarget, resources::ResourceState::ePresent);
		_rtvCommandList->ResourceBarrier(_resourceBarrier, 1);

		_rtvCommandList->Close();

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
		delete gDevice;
		for (size_t i = 0; i < FRAME_BUFFER_COUNT; i++)
		{
			delete _commandAllocators[i];
			delete _fences[i];
			delete _renderTargets[i];
		}
		delete _fenceEvent;
		delete _resourceBarrier;
		delete _swapChain;
		delete gClassHelper;
	}
}
