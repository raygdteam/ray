#include "pch.hpp"
#include "RendererDX.hpp"
#include "utils.hpp"

using namespace ray::renderer::directx11::utilities;

const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

namespace ray::renderer::directx11
{

	struct Vertex
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT4 Color;
	};

	RendererDX::RendererDX()
		: m_DriverType(D3D_DRIVER_TYPE_NULL)
		, m_FeatureLevel(D3D_FEATURE_LEVEL_11_0)
		, m_DepthStencilBuffer(nullptr)
		, m_DepthStencilState(nullptr)
		, m_DepthStencilView(nullptr)
		, m_RasterState(nullptr)
		, m_Fullscreen(false)
	{
	}

	bool RendererDX::Init()
	{
		spdlog::trace("renderer_dx: initializing...");

		HRESULT result;
		IDXGIFactory* factory;
		IDXGIAdapter* adapter;
		IDXGIOutput* adapterOutput;
		u32 numModes, i, numerator, denominator;
		size_t stringLength;
		DXGI_MODE_DESC* displayModeList;
		DXGI_ADAPTER_DESC adapterDesc;
		s32 error;
		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		D3D_FEATURE_LEVEL featureLevel;
		D3D11_TEXTURE2D_DESC depthBufferDesc;
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		D3D11_RASTERIZER_DESC rasterDesc;
		D3D11_VIEWPORT viewport;
		float fieldOfView, screenAspect;


		// Store the vsync setting.
		m_VSyncEnabled = true;

		// Create a DirectX graphics interface factory.
		result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
		if (FAILED(result))
		{
			return false;
		}

		// Use the factory to create an adapter for the primary graphics interface (video card).
		result = factory->EnumAdapters(0, &adapter);
		if (FAILED(result))
		{
			return false;
		}

		// Enumerate the primary adapter output (monitor).
		result = adapter->EnumOutputs(0, &adapterOutput);
		if (FAILED(result))
		{
			return false;
		}

		// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
		result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
		if (FAILED(result))
		{
			return false;
		}

		// Create a list to hold all the possible display modes for this monitor/video card combination.
		displayModeList = new DXGI_MODE_DESC[numModes];
		if (!displayModeList)
		{
			return false;
		}

		// Now fill the display mode list structures.
		result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
		if (FAILED(result))
		{
			return false;
		}

		// Now go through all the display modes and find the one that matches the screen width and height.
		// When a match is found store the numerator and denominator of the refresh rate for that monitor.
		for (i = 0; i < numModes; i++)
		{
			if (displayModeList[i].Width == (u32)Platform::GetWidth())
			{
				if (displayModeList[i].Height == (u32)Platform::GetHeight())
				{
					numerator = displayModeList[i].RefreshRate.Numerator;
					denominator = displayModeList[i].RefreshRate.Denominator;
				}
			}
		}

		// Get the adapter (video card) description.
		result = adapter->GetDesc(&adapterDesc);
		if (FAILED(result))
		{
			return false;
		}

		// Store the dedicated video card memory in bytes.
		m_VideoCardMemory = (int)(adapterDesc.DedicatedVideoMemory);
		// Convert the name of the video card to a character array and store it.
		error = wcstombs_s(&stringLength, m_VideoCardDescr, 128, adapterDesc.Description, 128);
		if (error != 0)
		{
			return false;
		}

		spdlog::trace("renderer_dx: physical device: {}", m_VideoCardDescr);
		spdlog::trace("renderer_dx: video memory size: {} MB", m_VideoCardMemory / 1024 / 1024);

		// Release the display mode list.
		delete[] displayModeList;
		displayModeList = 0;

		// Release the adapter output.
		adapterOutput->Release();
		adapterOutput = 0;

		// Release the adapter.
		adapter->Release();
		adapter = 0;

		// Release the factory.
		factory->Release();
		factory = 0;

		// Initialize the swap chain description.
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

		// Set to a single back buffer.
		swapChainDesc.BufferCount = 1;

		// Set the width and height of the back buffer.
		swapChainDesc.BufferDesc.Width = Platform::GetWidth();
		swapChainDesc.BufferDesc.Height = Platform::GetHeight();

		// Set regular 32-bit surface for the back buffer.
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		// Set the refresh rate of the back buffer.
		if (m_VSyncEnabled)
		{
			swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
		}
		else
		{
			swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		}

		// Set the usage of the back buffer.
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

		// Set the handle for the window to render to.
		swapChainDesc.OutputWindow = Platform::GetHWND();

		// Turn multisampling off.
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;

		// Set to full screen or windowed mode.
		if (m_Fullscreen)
		{
			swapChainDesc.Windowed = false;
		}
		else
		{
			swapChainDesc.Windowed = true;
		}

		// Set the scan line ordering and scaling to unspecified.
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		// Discard the back buffer contents after presenting.
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		// Don't set the advanced flags.
		swapChainDesc.Flags = 0;

		// Set the feature level to DirectX 11.
		featureLevel = D3D_FEATURE_LEVEL_11_0;

		// Create the swap chain, Direct3D device, and Direct3D device context.
		result = InitDirectXGlobalVariables(swapChainDesc);
		if (FAILED(result))
		{
			return false;
		}

		// Initialize the description of the depth buffer.
		ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

		// Set up the description of the depth buffer.
		depthBufferDesc.Width = Platform::GetWidth();
		depthBufferDesc.Height = Platform::GetHeight();
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;

		// Create the texture for the depth buffer using the filled out description.
		result = GetDevice()->CreateTexture2D(&depthBufferDesc, NULL, &m_DepthStencilBuffer);
		if (FAILED(result))
		{
			return false;
		}

		// Initialize the description of the stencil state.
		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

		// Set up the description of the stencil state.
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;

		// Stencil operations if pixel is front-facing.
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Stencil operations if pixel is back-facing.
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Create the depth stencil state.
		result = GetDevice()->CreateDepthStencilState(&depthStencilDesc, &m_DepthStencilState);
		if (FAILED(result))
		{
			return false;
		}

		// Initialize the depth stencil view.
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

		// Set up the depth stencil view description.
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		// Create the depth stencil view.
		result = GetDevice()->CreateDepthStencilView(m_DepthStencilBuffer, &depthStencilViewDesc, &m_DepthStencilView);
		if (FAILED(result))
		{
			return false;
		}

		ID3D11RenderTargetView* RenderTargetView = GetRenderTargetView();

		// Bind the render target view and depth stencil buffer to the output render pipeline.
		GetDeviceContext()->OMSetRenderTargets(1, &RenderTargetView, m_DepthStencilView);

		// Setup the raster description which will determine how and what polygons will be drawn.
		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.CullMode = D3D11_CULL_BACK;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.FrontCounterClockwise = false;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0.0f;

		// Create the rasterizer state from the description we just filled out.
		result = GetDevice()->CreateRasterizerState(&rasterDesc, &m_RasterState);
		if (FAILED(result))
		{
			return false;
		}

		// Now set the rasterizer state.
		GetDeviceContext()->RSSetState(m_RasterState);

		// Setup the viewport for rendering.
		viewport.Width = (float)Platform::GetWidth();
		viewport.Height = (float)Platform::GetHeight();
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;

		// Create the viewport.
		GetDeviceContext()->RSSetViewports(1, &viewport);

		// Setup the projection matrix.
		fieldOfView = 3.141592654f / 4.0f;
		screenAspect = (float)Platform::GetWidth() / (float)Platform::GetHeight();

		// Create the projection matrix for 3D rendering.
		m_ProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, SCREEN_NEAR, SCREEN_DEPTH);

		// Initialize the world matrix to the identity matrix.
		m_WorldMatrix = DirectX::XMMatrixIdentity();

		// Create an orthographic projection matrix for 2D rendering.
		m_OrthoMatrix = DirectX::XMMatrixOrthographicLH((float)Platform::GetWidth(), (float)Platform::GetHeight(), SCREEN_NEAR, SCREEN_DEPTH);

		D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
		D3D11_SUBRESOURCE_DATA vertexData, indexData;
		D3D11_INPUT_ELEMENT_DESC polygonLayout[2];

		// Create the vertex input layout description.
		polygonLayout[0].SemanticName = "POSITION";
		polygonLayout[0].SemanticIndex = 0;
		polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		polygonLayout[0].InputSlot = 0;
		polygonLayout[0].AlignedByteOffset = 0;
		polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[0].InstanceDataStepRate = 0;

		polygonLayout[1].SemanticName = "COLOR";
		polygonLayout[1].SemanticIndex = 0;
		polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		polygonLayout[1].InputSlot = 0;
		polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[1].InstanceDataStepRate = 0;

		shader.Create(eShaderTypeDX::Vertex, TEXT("E:\\Projects\\Ray\\ray\\Game\\resources\\shaders\\dx\\VertexShader.hlsl"));
		shader.Create(eShaderTypeDX::Pixel, TEXT("E:\\Projects\\Ray\\ray\\Game\\resources\\shaders\\dx\\PixelShader.hlsl"));

		shader.SetInputElement(polygonLayout, sizeof(polygonLayout) / sizeof(polygonLayout[0]));

		Vertex vertices[] =
		{
			{ DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) }, // Bottom left.
			{ DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) }, // Top middle.
			{ DirectX::XMFLOAT3(1.f, -1.f, 0.f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) } // Bottom right.
		};

		u32 indices[] =
		{
			0, 1, 2
		};

		DirectX::XMMATRIX Model = DirectX::XMMatrixIdentity();
		shader.SetParams(sizeof(indices) / sizeof(u32), Model, m_WorldMatrix, m_ProjectionMatrix);

		// Set up the description of the static vertex buffer.
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(vertices);
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the vertex data.
		vertexData.pSysMem = vertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		// Now create the vertex buffer.
		result = GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &VertexBuffer);
		if (FAILED(result))
		{
			return false;
		}

		// Set up the description of the static index buffer.
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(indices);
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the index data.
		indexData.pSysMem = indices;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		// Create the index buffer.
		result = GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &IndexBuffer);
		if (FAILED(result))
		{
			return false;
		}

		return true;
	}

	void RendererDX::Draw()
	{
		unsigned int stride;
		unsigned int offset;


		// Set vertex buffer stride and offset.
		stride = sizeof(Vertex);
		offset = 0;

		// Set the vertex buffer to active in the input assembler so it can be rendered.
		GetDeviceContext()->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);

		// Set the index buffer to active in the input assembler so it can be rendered.
		GetDeviceContext()->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
		GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		shader.UseShader();
	}

	void RendererDX::Destroy()
	{
		// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
		if (GetSwapChain())
		{
			GetSwapChain()->SetFullscreenState(false, NULL);
		}

		if (m_RasterState)
		{
			m_RasterState->Release();
			m_RasterState = 0;
		}

		if (m_DepthStencilView)
		{
			m_DepthStencilView->Release();
			m_DepthStencilView = 0;
		}

		if (m_DepthStencilState)
		{
			m_DepthStencilState->Release();
			m_DepthStencilState = 0;
		}

		if (m_DepthStencilBuffer)
		{
			m_DepthStencilBuffer->Release();
			m_DepthStencilBuffer = 0;
		}

		// Release the index buffer.
		if (IndexBuffer)
		{
			IndexBuffer->Release();
			IndexBuffer = 0;
		}

		// Release the vertex buffer.
		if (VertexBuffer)
		{
			VertexBuffer->Release();
			VertexBuffer = 0;
		}

		DestroyDirectXGlobalVariables();
	}

	void RendererDX::BeginFrame()
	{
		float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };

		// Clear the back buffer.
		GetDeviceContext()->ClearRenderTargetView(GetRenderTargetView(), ClearColor);

		// Clear the depth buffer.
		GetDeviceContext()->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	void RendererDX::EndFrame()
	{
		// Present the back buffer to the screen since rendering is complete.
		if (m_VSyncEnabled)
		{
			// Lock to screen refresh rate.
			GetSwapChain()->Present(1, 0);
		}
		else
		{
			// Present as fast as possible.
			GetSwapChain()->Present(0, 0);
		}
	}

}

