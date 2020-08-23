#include "renderer.hpp"
#include "command_context.hpp"
#include "command_queue.hpp"
#include "resources/buffer_manager.hpp"
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

		_currentBuffer = 0;

		struct Vertex
		{
			FVector<3> pos;
			FVector<4> color;
		};

		Vertex data[4] =
		{
			{ -0.5f,  0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f },
			{  0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f },
			{ -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f },
			{  0.5f,  0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 1.0f }
		};

		u32 indices[] =
		{
			0, 1, 2,
			0, 3, 1
		};

		_vertexBuffer.Create(4, sizeof(Vertex), static_cast<const void*>(data));
		_indexBuffer.Create(6, sizeof(u32), static_cast<const void*>(indices));
		_rootSignature.Finalize(D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
		_pipeline.SetRootSignature(_rootSignature);

		ID3DBlob* vertexShader;
		ID3DBlob* errorBuff;
		hr = D3DCompileFromFile(L"..\\..\\source\\engine\\modules\\renderer_core\\VertexShader.hlsl", nullptr, nullptr, "main", "vs_5_0",
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vertexShader, &errorBuff);
		check(hr == S_OK)

		D3D12_SHADER_BYTECODE vertexShaderBytecode = {};
		vertexShaderBytecode.BytecodeLength = vertexShader->GetBufferSize();
		vertexShaderBytecode.pShaderBytecode = vertexShader->GetBufferPointer();

		ID3DBlob* pixelShader;
		hr = D3DCompileFromFile(L"..\\..\\source\\engine\\modules\\renderer_core\\PixelShader.hlsl", nullptr, nullptr, "main", "ps_5_0",
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &pixelShader, &errorBuff);
		check(hr == S_OK)

		D3D12_SHADER_BYTECODE pixelShaderBytecode = {};
		pixelShaderBytecode.BytecodeLength = pixelShader->GetBufferSize();
		pixelShaderBytecode.pShaderBytecode = pixelShader->GetBufferPointer();

		_pipeline.SetVertexShader(vertexShaderBytecode);
		_pipeline.SetPixelShader(pixelShaderBytecode);

		D3D12_INPUT_ELEMENT_DESC inputLayout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
		};
		_pipeline.SetInputLayout(2, inputLayout);
		_pipeline.SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
		_pipeline.SetRenderTargetFormat(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_UNKNOWN);
		_pipeline.SetSampleMask(0xffffffff);
		_pipeline.SetRasterizerState(CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT));
		_pipeline.SetBlendState(CD3DX12_BLEND_DESC(D3D12_DEFAULT));
		_pipeline.Finalize();

		_vertexBufferView = _vertexBuffer.VertexBufferView(0);
		_indexBufferView = _indexBuffer.IndexBufferView(0);
}

	void IRenderer::BeginScene(GraphicsContext& gfxContext)
	{
		gfxContext.SetRootSignature(_rootSignature);
		gfxContext.SetScissor(0, 0, 1280, 720);
		gfxContext.SetViewport(0.f, 0.f, 1280.f, 720.f);
		
		gfxContext.TransitionResource(globals::gDisplayPlane[_currentBuffer], D3D12_RESOURCE_STATE_RENDER_TARGET, true);
		gfxContext.SetRenderTarget(globals::gDisplayPlane[_currentBuffer].GetRTV());
		gfxContext.ClearColor(globals::gDisplayPlane[_currentBuffer]);
		
		gfxContext.SetPipelineState(_pipeline);
		gfxContext.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		struct Vertex
		{
			FVector<3> pos;
			FVector<4> color;
		};

		Vertex data[4] =
		{
			{ -0.5f,  0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f },
			{  0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f },
			{ -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f },
			{  0.5f,  0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 1.0f }
		};
		gfxContext.SetDynamicVB(0, 4, sizeof(Vertex), data);
		//gfxContext.SetVertexBuffer(0, _vertexBufferView);
		gfxContext.SetIndexBuffer(_indexBufferView);
		gfxContext.DrawIndexedInstanced(_indexBuffer.GetElementCount(), 1, 0, 0, 0);
		
		gfxContext.TransitionResource(globals::gDisplayPlane[_currentBuffer], D3D12_RESOURCE_STATE_PRESENT);
		gfxContext.Finish(true);

		_swapChain->Present(0, 0);
		_currentBuffer = (_currentBuffer + 1) % globals::SWAP_CHAIN_BUFFER_COUNT;
	} 

	void IRenderer::EndScene(GraphicsContext& gfxContext)
	{
		gfxContext.GetCommandList();
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
