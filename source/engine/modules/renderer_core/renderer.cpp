#include "renderer.hpp"
#include "command_context.hpp"
#include "command_queue.hpp"
#include "resources/buffer_manager.hpp"
#include "resources/graphics_memory_manager.hpp"
#include "d3dx12.h"
#include <core/math/vector.hpp>

//ñäåëàéòå óæå èíòåðôåéñ äëÿ ðàáîòû ñ ìîäóëÿìè
#include <Windows.h>
#include <d3dcompiler.h>

#include <engine/state/state.hpp>

namespace ray::renderer_core_api
{
	namespace globals
	{
		CommandListManager gCommandListManager;
		ContextManager gContextManager;
		ID3D12Device* gDevice;
		DescriptorAllocator gDescriptorAllocator[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES] =
		{
			D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
			D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER,
			D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
			D3D12_DESCRIPTOR_HEAP_TYPE_DSV
		};
		ray::renderer_core_api::resources::GraphicsMemoryManager gGMemManager;
	}


	bool IRenderer::_sbReady = false;

	void IRenderer::Initialize(ray::core::IPlatformWindow* window)
	{
		ray_assert(_swapChain == nullptr, "Renderer has been already initialized")

		ID3D12Debug* spDebugController0;
		ID3D12Debug1* spDebugController1;
		check(D3D12GetDebugInterface(IID_PPV_ARGS(&spDebugController0)) == S_OK)
		check(spDebugController0->QueryInterface(IID_PPV_ARGS(&spDebugController1)) == S_OK);
		// spDebugController1->SetEnableGPUBasedValidation(true);
		//auto hr = globals::gDevice->GetDeviceRemovedReason();
		//u32 code = static_cast<u32>(hr);
		spDebugController0->EnableDebugLayer();

		IDXGIFactory4* dxgiFactory;
		auto hr = CreateDXGIFactory2(0, IID_PPV_ARGS(&dxgiFactory));
		(void)hr;
		assert(hr == S_OK);
		IDXGIAdapter1* adapter;
		ID3D12Device* device;

		size_t maxSize = 0;

		for (uint32_t i = 0; DXGI_ERROR_NOT_FOUND != dxgiFactory->EnumAdapters1(i, &adapter); ++i)
		{
			DXGI_ADAPTER_DESC1 desc;
			adapter->GetDesc1(&desc);
			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
				continue;

			if (desc.DedicatedVideoMemory > maxSize && D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device)) == S_OK)
			{
				adapter->GetDesc1(&desc);
				maxSize = desc.DedicatedVideoMemory;
			}
		}

		if (maxSize > 0)
			globals::gDevice = device;

		globals::gCommandListManager.Create(globals::gDevice);

		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		swapChainDesc.Width = window->GetWidth();
		swapChainDesc.Height = window->GetHeight();
		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.Scaling = DXGI_SCALING_NONE;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = globals::SWAP_CHAIN_BUFFER_COUNT;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
		 
		hr = dxgiFactory->CreateSwapChainForHwnd(globals::gCommandListManager.GetCommandQueue(), static_cast<HWND>(window->GetWindowHandleRaw()), &swapChainDesc, nullptr, nullptr, &_swapChain);
		auto code = static_cast<u32>(hr);
		if(code == 0) {  }
		for (u32 i = 0; i < globals::SWAP_CHAIN_BUFFER_COUNT; ++i)
		{
			ID3D12Resource* displayPlane;
			_swapChain->GetBuffer(i, IID_PPV_ARGS(&displayPlane));
			globals::gDisplayPlane[i].CreateFromSwapChain(displayPlane);
			//displayPlane->Release();
		}
		
		globals::gCurrentBuffer = 0;
		globals::gDepthBuffer.Create(globals::gDisplayPlane->GetWidth(), globals::gDisplayPlane->GetHeight(), DXGI_FORMAT_D32_FLOAT);
		globals::gGMemManager.Initialize();
	}

	void IRenderer::BeginScene(GraphicsContext& gfxContext)
	{
		gfxContext.TransitionResource(globals::gDisplayPlane[globals::gCurrentBuffer], D3D12_RESOURCE_STATE_RENDER_TARGET, true);
		gfxContext.TransitionResource(globals::gDepthBuffer, D3D12_RESOURCE_STATE_DEPTH_WRITE, true);
		gfxContext.SetRenderTarget(globals::gDisplayPlane[globals::gCurrentBuffer].GetRTV(), globals::gDepthBuffer.GetDSV());
		gfxContext.ClearColor(globals::gDisplayPlane[globals::gCurrentBuffer]);
		gfxContext.ClearDepthAndStencil(globals::gDepthBuffer);
	} 

	void IRenderer::EndScene(GraphicsContext& gfxContext)
	{
		gfxContext.TransitionResource(globals::gDepthBuffer, D3D12_RESOURCE_STATE_DEPTH_READ, true);
		gfxContext.TransitionResource(globals::gDisplayPlane[globals::gCurrentBuffer], D3D12_RESOURCE_STATE_PRESENT);
		gfxContext.Finish(true);

		_swapChain->Present(0, 0);
		globals::gCurrentBuffer = (globals::gCurrentBuffer + 1) % globals::SWAP_CHAIN_BUFFER_COUNT;
	}

	void IRenderer::Shutdown()
	{
		CommandContext::DestroyAllContexts();
		globals::gCommandListManager.Shutdown();
		DescriptorAllocator::DestroyAll();

		_swapChain->Release();
		_swapChain = nullptr;
		for (size_t i = 0; i < globals::SWAP_CHAIN_BUFFER_COUNT; ++i)
			globals::gDisplayPlane[i].Destroy();
		
	}
}
