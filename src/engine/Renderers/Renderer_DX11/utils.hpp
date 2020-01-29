#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

namespace ray::renderer::directx11::utilities
{

	struct Vertex
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT4 Color;
	};
	
	ID3D11Device* GetDevice();
	IDXGISwapChain* GetSwapChain();
	void DestroyDirectXGlobalVariables();
	ID3D11DeviceContext* GetDeviceContext();
	ID3D11RenderTargetView* GetRenderTargetView();
	HRESULT CreateDeviceAndContextDevice();
	HRESULT CreateSwapChain(DXGI_SWAP_CHAIN_DESC&);
	HRESULT CreateRenderTargetView();
	HRESULT CreateVertexBuffer(ID3D11Buffer**, Vertex*, size_t);
	HRESULT CreateIndexBuffer(ID3D11Buffer**, u32*, size_t);
}
