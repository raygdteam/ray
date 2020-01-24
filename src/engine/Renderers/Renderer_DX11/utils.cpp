#include "pch.hpp"
#include "utils.hpp"

namespace ray::renderer::directx11::utilities
{
	ID3D11Device* gDevice = nullptr;
	IDXGISwapChain* gSwapChain = nullptr;
	ID3D11Texture2D* gBackBuffer = nullptr;
	ID3D11DeviceContext* gDeviceContext = nullptr;
	ID3D11RenderTargetView* gRenderTargetView = nullptr;

	HRESULT CreateDeviceAndContextDevice()
	{
		u32 Flags = 0;
#ifdef RAY_DEBUG
		//Flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif //RAY_DEBUG
		D3D_FEATURE_LEVEL FeatureLevel = D3D_FEATURE_LEVEL_11_0;

		return D3D11CreateDevice(
			0, // default adapter
			D3D_DRIVER_TYPE_HARDWARE,
			0, // no software device
			Flags,
			0, 0, // default feature level array
			D3D11_SDK_VERSION,
			&gDevice,
			&FeatureLevel,
			&gDeviceContext);
	}

	HRESULT CreateSwapChain(DXGI_SWAP_CHAIN_DESC& SwapChainDesc)
	{
		IDXGIDevice* dxgiDevice = 0;
		gDevice->QueryInterface(__uuidof(IDXGIDevice),
			(void**)&dxgiDevice);
		IDXGIAdapter* dxgiAdapter = 0;
		dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);
		
		// Finally got the IDXGIFactory interface.
		IDXGIFactory* dxgiFactory = 0;
		dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);
		
		// Now, create the swap chain.
		IDXGISwapChain* mSwapChain;
		HRESULT hr = dxgiFactory->CreateSwapChain(gDevice, &SwapChainDesc, &gSwapChain);
		
		// Release our acquired COM interfaces (because we are done with them).
		dxgiDevice->Release();
		dxgiAdapter->Release();
		dxgiFactory->Release();

		return hr;
	}

	HRESULT CreateRenderTargetView()
	{
		gSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&gBackBuffer);
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