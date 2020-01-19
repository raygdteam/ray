#include "pch.hpp"
#include "utils.hpp"

namespace ray::renderer::directx11::utilities
{
	ID3D11Device* gDevice = nullptr;
	IDXGISwapChain* gSwapChain = nullptr;
	ID3D11Texture2D* gBackBuffer = nullptr;
	ID3D11DeviceContext* gDeviceContext = nullptr;
	ID3D11RenderTargetView* gRenderTargetView = nullptr;

	HRESULT CreateDeviceAndSwapChain(DXGI_SWAP_CHAIN_DESC&);
	HRESULT CreateRenderTargetView();

	HRESULT InitDirectXGlobalVariables(DXGI_SWAP_CHAIN_DESC& SwapChainDesc)
	{
		if (gDevice || gDeviceContext || gRenderTargetView || gSwapChain)
			return S_FALSE;

		HRESULT CreateDeviceAndSwapChainResult, CreateRenderTargetViewResult;
		CreateDeviceAndSwapChainResult = CreateDeviceAndSwapChain(SwapChainDesc);
		gSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&gBackBuffer);
		CreateRenderTargetViewResult = CreateRenderTargetView();
		
		return CreateDeviceAndSwapChainResult && CreateRenderTargetViewResult;
	}

	HRESULT CreateDeviceAndSwapChain(DXGI_SWAP_CHAIN_DESC& SwapChainDesc)
	{
		u32 Flags = 0;
#ifdef RAY_DEBUG
		//Flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif //RAY_DEBUG
		D3D_FEATURE_LEVEL FeatureLevel = D3D_FEATURE_LEVEL_11_0;
		return D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, Flags, &FeatureLevel, 1,
			D3D11_SDK_VERSION, &SwapChainDesc, &gSwapChain, &gDevice, NULL, &gDeviceContext);
	}

	HRESULT CreateRenderTargetView()
	{
		return gDevice->CreateRenderTargetView(gBackBuffer, NULL, &gRenderTargetView);
	}

	void DestroyDirectXGlobalVariables()
	{
		if (gRenderTargetView)
		{
			gRenderTargetView->Release();
			gRenderTargetView = 0;
		}

		if (gDeviceContext)
		{
			gDeviceContext->ClearState();
			gDeviceContext->Release();
			gDeviceContext = 0;
		}

		if (gDevice)
		{
			gDevice->Release();
			gDevice = 0;
		}

		if (gSwapChain)
		{
			gSwapChain->Release();
			gSwapChain = 0;
		}
	}

	ID3D11Device* GetDevice()
	{
		return gDevice;
	}

	IDXGISwapChain* GetSwapChain()
	{
		return gSwapChain;
	}

	ID3D11DeviceContext* GetDeviceContext()
	{
		return gDeviceContext;
	}

	ID3D11RenderTargetView* GetRenderTargetView()
	{
		return gRenderTargetView;
	}

}