#include "renderer.hpp"
#include "command_context.hpp"
#include "command_queue.hpp"
#include "resources/buffer_manager.hpp"
#include "resources/gpu_texture.hpp"
#include "resources/gpu_buffer.hpp"
#include "resources/gpu_resource.hpp"
#include "resources/ring_buffer.hpp"
#include "resources/color_buffer.hpp"
#include "resources/upload_buffer.hpp"
#include "d3dx12.h"
#include <core/math/vector.hpp>
#include "pipeline_state.hpp"
#include "root_signature.hpp"
#include "sampler_manager.hpp"

//ñäåëàéòå óæå èíòåðôåéñ äëÿ ðàáîòû ñ ìîäóëÿìè
#include <Windows.h>
#include <d3dcompiler.h>

#include <engine/state/state.hpp>

CommandListManager gCommandListManager;
ContextManager gContextManager;
ID3D12Device* gDevice;
GpuResourceAllocator<GpuTextureMemoryPool> gTextureAllocator;
GpuResourceAllocator<GpuBufferMemoryPool> gBufferAllocator;
GpuResourceAllocator<GpuPixelBufferMemoryPool> gPixelBufferAllocator;
RingBuffer gRingBuffer;
RAY_RENDERERCORE_API UploadBuffer* gUploadBuffer;

RendererStats_t sRendererStats;

DescriptorAllocator gDescriptorAllocator[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES] =
{
	D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
	D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER,
	D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
	D3D12_DESCRIPTOR_HEAP_TYPE_DSV
};

bool IRenderer::_sbReady = false;
RendererStats_t IRenderer::sRendererStats;
RendererInfo_t IRenderer::sRendererInfo;

void IRenderer::Initialize(IPlatformWindow* window) noexcept
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
	check(hr == S_OK);
	IDXGIAdapter1* adapter;
	ID3D12Device* device;

	size_t maxSize = 0;

	for (u32 i = 0; DXGI_ERROR_NOT_FOUND != dxgiFactory->EnumAdapters1(i, &adapter); ++i)
	{
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);
		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			continue;

		if (desc.DedicatedVideoMemory >= maxSize && D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device)) == S_OK)
		{
			adapter->GetDesc1(&desc);
			maxSize = desc.DedicatedVideoMemory;
		}
	}

	if (maxSize > 0)
	{
		gDevice = device;
	}
	else
	{
		ray_assert(false, "There are no any supported devices.")
	}

	sRendererInfo.MaxGpuMemorySize = maxSize;

	D3D12_FEATURE_DATA_D3D12_OPTIONS  options;
	gDevice->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS, &options, sizeof(options));
	sRendererInfo.bHeapTier2 = options.ResourceHeapTier == D3D12_RESOURCE_HEAP_TIER_2;

	gCommandListManager.Create(gDevice);

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.Width = window->GetClientWidth();
	swapChainDesc.Height = window->GetClientHeight();
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.Scaling = DXGI_SCALING_NONE;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = SWAP_CHAIN_BUFFER_COUNT;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

	hr = dxgiFactory->CreateSwapChainForHwnd(gCommandListManager.GetCommandQueue(), static_cast<HWND>(window->GetWindowHandleRaw()), &swapChainDesc, nullptr, nullptr, &_swapChain);
	for (u32 i = 0; i < SWAP_CHAIN_BUFFER_COUNT; ++i)
	{
		ID3D12Resource* displayPlane;
		_swapChain->GetBuffer(i, IID_PPV_ARGS(&displayPlane));
		gDisplayPlane[i].CreateFromSwapChain(displayPlane, "gDisplayPlane");
	}

	gCurrentBuffer = 0;

	gUploadBuffer = new UploadBuffer;
	gUploadBuffer->Initialize(MB(64));

	gPixelBufferAllocator.Initialize(MB(8));
	gTextureAllocator.Initialize(MB(10));
	gBufferAllocator.Initialize(MB(10));

	gDepthBuffer.Create(gDisplayPlane->GetDesc().Width, gDisplayPlane->GetDesc().Height, DXGI_FORMAT_D32_FLOAT, "gDepthBuffer");
	gSceneColorBuffer.Create(window->GetClientWidth(), window->GetClientHeight(), 1, DXGI_FORMAT_R8G8B8A8_UNORM, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET, "gSceneColorBuffer");
	gEditorColorBuffer.Create(window->GetClientWidth(), window->GetClientHeight(), 1, DXGI_FORMAT_R8G8B8A8_UNORM, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET, "gEditorColorBuffer");
	
	gRingBuffer.Initialize(MB(15));

	PreparePresentObjects();
	
	Function<void(u32, u32)> a([this](u32 w, u32 h)
	{
		OnResize(w, h);
	});
	
	window->RegisterWindowResizeEventCallback(a);
}

struct PresentVertex
{
	FVector3 Position;
	FVector2 TexCoords;
};

void IRenderer::PreparePresentObjects() noexcept
{
	_srvDescriptorHeap.Create();

	PresentVertex vertices[] =
	{
		{ {-1.f, -1.f, 1.f}, {0.f, 1.f} },
		{ {-1.f, 1.f, 1.f}, {0.f, 0.f} },
		{ {1.f, 1.f, 1.f}, {1.f, 0.f} },
		{ {1.f, -1.f, 1.f}, {1.f, 1.f} }
	};

	auto vbDesc = GpuBufferDescription::Vertex(4, sizeof(PresentVertex));
	gUploadBuffer->SetBufferData(vbDesc, vertices);
	vbDesc.Flags = D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;
	_presentVB.Create(vbDesc, "IRenderer::_presentVB");
	_vbView.Create(_presentVB);

	u32 indices[] =
	{
		0, 1, 2,
		2, 3, 0
	};

	auto ibDesc = GpuBufferDescription::Index(sizeof(indices) / sizeof(u32), sizeof(u32));
	gUploadBuffer->SetBufferData(ibDesc, indices);
	ibDesc.Flags = D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;
	_presentIB.Create(ibDesc, "IRenderer::_presentIB");
	_ibView.Create(_presentIB);
	
	SamplerDesc sampler;
	_presentSignature.Begin(1, 1);
	_presentSignature.Slot(0).InitAsDescriptorRange(0, D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, D3D12_SHADER_VISIBILITY_PIXEL);
	_presentSignature.InitStaticSampler(0, sampler, D3D12_SHADER_VISIBILITY_PIXEL);
	_presentSignature.Finalize(D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	_presentPipeline.SetRootSignature(_presentSignature);

	ID3DBlob* vertexShader;
	ID3DBlob* errorBuff;
	MAYBE_UNUSED auto hr = D3DCompileFromFile(L"..\\..\\source\\engine\\modules\\renderer_core\\Present_VS.hlsl", nullptr, nullptr, "main", "vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vertexShader, &errorBuff);
	check(hr == S_OK)

		D3D12_SHADER_BYTECODE vertexShaderBytecode = {};
	vertexShaderBytecode.BytecodeLength = vertexShader->GetBufferSize();
	vertexShaderBytecode.pShaderBytecode = vertexShader->GetBufferPointer();

	ID3DBlob* pixelShader;
	hr = D3DCompileFromFile(L"..\\..\\source\\engine\\modules\\renderer_core\\Present_PS.hlsl", nullptr, nullptr, "main", "ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &pixelShader, &errorBuff);
	check(hr == S_OK)

		D3D12_SHADER_BYTECODE pixelShaderBytecode = {};
	pixelShaderBytecode.BytecodeLength = pixelShader->GetBufferSize();
	pixelShaderBytecode.pShaderBytecode = pixelShader->GetBufferPointer();

	_presentPipeline.SetVertexShader(vertexShaderBytecode);
	_presentPipeline.SetPixelShader(pixelShaderBytecode);

	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};
	_presentPipeline.SetInputLayout(sizeof(inputLayout) / sizeof(D3D12_INPUT_ELEMENT_DESC), inputLayout);
	_presentPipeline.SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	_presentPipeline.SetRenderTargetFormat(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_UNKNOWN);
	_presentPipeline.SetSampleMask(0xffffffff);
	_presentPipeline.SetRasterizerState(CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT));
	_presentPipeline.SetBlendState(CD3DX12_BLEND_DESC(D3D12_DEFAULT));

	_presentPipeline.SetDepthStencilState(CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT));
	//_presentPipeline.SetDSVFormat(gDepthBuffer.GetDesc().Format);

	_presentPipeline.Finalize();
}

void IRenderer::Begin(ColorBuffer& renderTarget, GraphicsContext& gfxContext) noexcept
{
	gfxContext.TransitionResource(gDepthBuffer, D3D12_RESOURCE_STATE_DEPTH_WRITE, true);
	gfxContext.TransitionResource(renderTarget, D3D12_RESOURCE_STATE_RENDER_TARGET, true);

	gfxContext.ClearColor(renderTarget);
	gfxContext.ClearDepthAndStencil(gDepthBuffer);
}

void IRenderer::End(ColorBuffer& renderTarget, GraphicsContext& gfxContext) noexcept
{
	/*gfxContext.TransitionResource(gDepthBuffer, D3D12_RESOURCE_STATE_DEPTH_READ, true);
	gfxContext.TransitionResource(renderTarget, D3D12_RESOURCE_STATE_PRESENT);*/
	
	gfxContext.Flush();
}

void IRenderer::Present(ColorBuffer& finalFrame, GraphicsContext& gfxContext) noexcept
{
	gfxContext.TransitionResource(finalFrame, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, true);
	gfxContext.TransitionResource(gDisplayPlane[gCurrentBuffer], D3D12_RESOURCE_STATE_RENDER_TARGET, true);
	auto frameSrv = finalFrame.GetSRV();
	auto destSrv = _srvDescriptorHeap.GetDescriptorAtOffset(0).GetCpuHandle();
	u32 rangeSize = 1;
	gDevice->CopyDescriptors(1, &destSrv, &rangeSize, 1, &frameSrv, &rangeSize, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	gfxContext.SetRootSignature(_presentSignature);
	gfxContext.SetScissor(0, 0, gDisplayPlane->GetDesc().Width, gDisplayPlane->GetDesc().Height);
	gfxContext.SetViewport(0.f, 0.f, gDisplayPlane->GetDesc().Width, gDisplayPlane->GetDesc().Height);

	gfxContext.SetPipelineState(_presentPipeline);
	gfxContext.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	gfxContext.SetRenderTarget(gDisplayPlane[gCurrentBuffer].GetRTV());

	gfxContext.SetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, _srvDescriptorHeap.GetHeapPointer());
	gfxContext.SetDescriptorTable(0, _srvDescriptorHeap.GetDescriptorAtOffset(0).GetGpuHandle());

	auto vb = _vbView.GetVertexBufferView();
	auto ib = _ibView.GetIndexBufferView();
	gfxContext.SetVertexBuffer(0, vb);
	gfxContext.SetIndexBuffer(ib);
	gfxContext.DrawIndexedInstanced(ib.SizeInBytes / sizeof(u32), 1, 0, 0, 0);

	gfxContext.TransitionResource(gDepthBuffer, D3D12_RESOURCE_STATE_DEPTH_READ, true);
	gfxContext.TransitionResource(gDisplayPlane[gCurrentBuffer], D3D12_RESOURCE_STATE_PRESENT);
	gfxContext.Finish(true);

	_swapChain->Present(0, 0);
	gCurrentBuffer = (gCurrentBuffer + 1) % SWAP_CHAIN_BUFFER_COUNT;
	sRendererStats.DrawCallsCount = 0u;
	gUploadBuffer->Reset();
}

void IRenderer::OnResize(u32 width, u32 height)
{
	_swapChain->ResizeBuffers(SWAP_CHAIN_BUFFER_COUNT, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
	for (size_t i = 0; i < SWAP_CHAIN_BUFFER_COUNT; ++i)
	{
		gDisplayPlane[i].Destroy();
		ID3D12Resource* displayPlane;
		_swapChain->GetBuffer(i, IID_PPV_ARGS(&displayPlane));
		gDisplayPlane[i].CreateFromSwapChain(displayPlane, "gDisplayPlane");
	}

	// There is no need to reset gEditorColorBuffer when we don't use Editor
	gEditorColorBuffer.Reset(width, height, "gEditorColorBuffer");
	gSceneColorBuffer.Reset(width, height, "gSceneColorBuffer");
}

void IRenderer::Shutdown() noexcept
{
	_presentVB.Destroy();
	_presentIB.Destroy();

	gUploadBuffer->Destroy();
	delete gUploadBuffer;

	CommandContext::DestroyAllContexts();
	gCommandListManager.Shutdown();
	gTextureAllocator.Destroy();
	gBufferAllocator.Destroy();

	_swapChain->Release();
	_swapChain = nullptr;
	for (size_t i = 0; i < SWAP_CHAIN_BUFFER_COUNT; ++i)
		gDisplayPlane[i].Destroy();

	gDepthBuffer.Destroy();

	gPixelBufferAllocator.Destroy();
}
