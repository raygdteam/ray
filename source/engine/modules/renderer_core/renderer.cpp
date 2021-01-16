#include "renderer.hpp"
#include "command_context.hpp"
#include "command_queue.hpp"
#include "resources/buffer_manager.hpp"
#include "resources/gpu_texture.hpp"
#include "resources/gpu_buffer.hpp"
#include "d3dx12.h"
#include <core/math/vector.hpp>

//ñäåëàéòå óæå èíòåðôåéñ äëÿ ðàáîòû ñ ìîäóëÿìè
#include <Windows.h>
#include <d3dcompiler.h>

#include <engine/state/state.hpp>

CommandListManager gCommandListManager;
ContextManager gContextManager;
ID3D12Device* gDevice;
DescriptorHeapsManager gDescriptorHeapsManager;
GpuTextureAllocator gTextureAllocator;
GpuBufferAllocator gBufferAllocator;

bool IRenderer::_sbReady = false;

void IRenderer::Initialize(IPlatformWindow* window)
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
		gDevice = device;

	gCommandListManager.Create(gDevice);

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

	hr = dxgiFactory->CreateSwapChainForHwnd(gCommandListManager.GetCommandQueue(), static_cast<HWND>(window->GetWindowHandleRaw()), &swapChainDesc, nullptr, nullptr, &_swapChain);
	auto code = static_cast<u32>(hr);
	if (code == 0) {}
	for (u32 i = 0; i < SWAP_CHAIN_BUFFER_COUNT; ++i)
	{
		ID3D12Resource* displayPlane;
		_swapChain->GetBuffer(i, IID_PPV_ARGS(&displayPlane));
		gDisplayPlane[i].CreateFromSwapChain(displayPlane);
		//displayPlane->Release();
	}

	gCurrentBuffer = 0;
	gDepthBuffer.Create(gDisplayPlane->GetWidth(), gDisplayPlane->GetHeight(), DXGI_FORMAT_D32_FLOAT);
	gTextureAllocator.Initialize(MB(4));
	//globals::gBufferAllocator.Initialize(MB(400));
}

void IRenderer::BeginScene(GraphicsContext& gfxContext)
{
	gfxContext.TransitionResource(gDisplayPlane[gCurrentBuffer], D3D12_RESOURCE_STATE_RENDER_TARGET, true);
	gfxContext.TransitionResource(gDepthBuffer, D3D12_RESOURCE_STATE_DEPTH_WRITE, true);
	gfxContext.SetRenderTarget(gDisplayPlane[gCurrentBuffer].GetRTV(), gDepthBuffer.GetDSV());
	gfxContext.ClearColor(gDisplayPlane[gCurrentBuffer]);
	gfxContext.ClearDepthAndStencil(gDepthBuffer);
}

void IRenderer::EndScene(GraphicsContext& gfxContext)
{
	gfxContext.TransitionResource(gDepthBuffer, D3D12_RESOURCE_STATE_DEPTH_READ, true);
	gfxContext.TransitionResource(gDisplayPlane[gCurrentBuffer], D3D12_RESOURCE_STATE_PRESENT);
	gfxContext.Finish(true);

	_swapChain->Present(0, 0);
	gCurrentBuffer = (gCurrentBuffer + 1) % SWAP_CHAIN_BUFFER_COUNT;
}

void IRenderer::Shutdown()
{
	CommandContext::DestroyAllContexts();
	gCommandListManager.Shutdown();
	gTextureAllocator.Destroy();
	gBufferAllocator.Destroy();

	_swapChain->Release();
	_swapChain = nullptr;
	for (size_t i = 0; i < SWAP_CHAIN_BUFFER_COUNT; ++i)
		gDisplayPlane[i].Destroy();

}
