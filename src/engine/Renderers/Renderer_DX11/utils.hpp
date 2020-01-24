#pragma once
#include <d3d11.h>

namespace ray::renderer::directx11::utilities
{
	
	ID3D11Device* GetDevice();
	IDXGISwapChain* GetSwapChain();
	void DestroyDirectXGlobalVariables();
	ID3D11DeviceContext* GetDeviceContext();
	ID3D11RenderTargetView* GetRenderTargetView();
	HRESULT CreateDeviceAndContextDevice();
	HRESULT CreateSwapChain(DXGI_SWAP_CHAIN_DESC&);
	HRESULT CreateRenderTargetView();
}
