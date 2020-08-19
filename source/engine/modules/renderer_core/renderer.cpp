#include "renderer.hpp"
#include "command_context.hpp"
#include "command_queue.hpp"

//ñäåëàéòå óæå èíòåðôåéñ äëÿ ðàáîòû ñ ìîäóëÿìè
#include <Windows.h>


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
	}


	bool IRenderer::_sbReady = false;

	void IRenderer::Initialize(ray::core::IPlatformWindow* window)
	{
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
		swapChainDesc.BufferCount = SWAP_CHAIN_BUFFER_COUNT;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
		 
		dxgiFactory->CreateSwapChainForHwnd(globals::gCommandListManager.GetCommandQueue(), static_cast<HWND>(window->GetWindowHandleRaw()), &swapChainDesc, nullptr, nullptr, &_swapChain);

		for (u32 i = 0; i < SWAP_CHAIN_BUFFER_COUNT; ++i)
		{
			ID3D12Resource* displayPlane;
			_swapChain->GetBuffer(i, IID_PPV_ARGS(&displayPlane));
			_displayPlane[i].CreateFromSwapChain(displayPlane);
			//displayPlane->Release();
		}

		_currentBuffer = 0;
	}

	void IRenderer::BeginScene()
	{
		GraphicsContext& gfxContext = GraphicsContext::Begin();
		gfxContext.TransitionResource(_displayPlane[_currentBuffer], D3D12_RESOURCE_STATE_RENDER_TARGET, true);
		gfxContext.SetRenderTarget(_displayPlane[_currentBuffer].GetRTV());
		gfxContext.ClearColor(_displayPlane[_currentBuffer]);
		gfxContext.TransitionResource(_displayPlane[_currentBuffer], D3D12_RESOURCE_STATE_PRESENT);

		gfxContext.Finish(true);

		_swapChain->Present(0, 0);
		_currentBuffer = (_currentBuffer + 1) % SWAP_CHAIN_BUFFER_COUNT;
	} 

	void IRenderer::EndScene()
	{
		
	}

	void IRenderer::Execute()
	{
		
	}


	void IRenderer::WaitForPreviousFrame()
	{
		
	}

	void IRenderer::Shutdown()
	{
		_swapChain->Release();
	}
}
