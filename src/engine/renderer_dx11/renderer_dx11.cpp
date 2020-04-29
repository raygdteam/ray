#include <renderer_core/renderer.hpp>
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>

#include <app_framework/base/platform_window.hpp>

using namespace ray;
using namespace ray::core;

class RendererD3D11 : public ray::IRenderer
{
public: 
	RendererD3D11();
	void Initialize(ray::core::IPlatformWindow*) override;
	void Draw() override;
	void Shutdown() override;

private:
	ID3D11Device* mDevice;
	ID3D11DeviceContext* mDeviceContext;
	ID3D11RenderTargetView* mRenderTargetView;
	IDXGISwapChain* mSwapChain;
	ID3D11Texture2D* mDepthStencilBuffer;
	ID3D11DepthStencilState* mDepthStencilState;
	ID3D11DepthStencilView* mDepthStencilView;

};

extern "C" __declspec(dllexport) ray::IRenderer * GetRendererApi()
{
	return new RendererD3D11;
}

RendererD3D11::RendererD3D11()
	: mDevice(nullptr)
	, mDeviceContext(nullptr)
	, mSwapChain(nullptr)
	, mRenderTargetView(nullptr)
	, mDepthStencilBuffer(nullptr)
	, mDepthStencilState(nullptr)
	, mDepthStencilView(nullptr)
{}

void RendererD3D11::Initialize(ray::core::IPlatformWindow* window)
{
	HRESULT Result;
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	D3D11_VIEWPORT Viewport;
	ID3D11Texture2D* BackBuffer;
	D3D_FEATURE_LEVEL FeatureLevel;
	D3D11_TEXTURE2D_DESC DepthStencilBufDesc;
	D3D11_DEPTH_STENCIL_DESC DepthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc;

	ZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));
	SwapChainDesc.BufferCount = 1;
	SwapChainDesc.BufferDesc.Width = 800;
	SwapChainDesc.BufferDesc.Height = 600;
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	//vsync off
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = 1;
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 0;

	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.OutputWindow = static_cast<HWND>(window->GetWindowHandleRaw());

	SwapChainDesc.SampleDesc.Count = 1;
	SwapChainDesc.SampleDesc.Quality = 0;

	SwapChainDesc.Windowed = false;

	// Set the scan line ordering and scaling to unspecified.
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	SwapChainDesc.Flags = 0;

	FeatureLevel = D3D_FEATURE_LEVEL_11_0;

	Result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, &FeatureLevel, 1, D3D11_SDK_VERSION, &SwapChainDesc, &mSwapChain, &mDevice, nullptr, &mDeviceContext);
	if (FAILED(Result))
		return;

	Result = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&BackBuffer));
	if (FAILED(Result))
		return;

	Result = mDevice->CreateRenderTargetView(BackBuffer, nullptr, &mRenderTargetView);
	if (FAILED(Result))
		return;

	BackBuffer->Release();
	BackBuffer = nullptr;
	
	// Initialize the description of the depth buffer.
	ZeroMemory(&DepthStencilBufDesc, sizeof(DepthStencilBufDesc));
	RECT rect;
	int Width = 0;
	int Height = 0;
	if (GetWindowRect(dynamic_cast<HWND>(static_cast<HWND>(window->GetWindowHandleRaw())), &rect))
	{
		Width = rect.right - rect.left;
		Height = rect.bottom - rect.top;
	}

	// Set up the description of the depth buffer.
	DepthStencilBufDesc.Width = Width;
	DepthStencilBufDesc.Height = Height;
	DepthStencilBufDesc.MipLevels = 1;
	DepthStencilBufDesc.ArraySize = 1;
	DepthStencilBufDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthStencilBufDesc.SampleDesc.Count = 1;
	DepthStencilBufDesc.SampleDesc.Quality = 0;
	DepthStencilBufDesc.Usage = D3D11_USAGE_DEFAULT;
	DepthStencilBufDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	DepthStencilBufDesc.CPUAccessFlags = 0;
	DepthStencilBufDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	Result = mDevice->CreateTexture2D(&DepthStencilBufDesc, NULL, &mDepthStencilBuffer);
	if (FAILED(Result))
	{
		return;
	}

	// Initialize the description of the stencil state.
	ZeroMemory(&DepthStencilDesc, sizeof(DepthStencilDesc));

	// Set up the description of the stencil state.
	DepthStencilDesc.DepthEnable = true;
	DepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	DepthStencilDesc.StencilEnable = true;
	DepthStencilDesc.StencilReadMask = 0xFF;
	DepthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	DepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	DepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	DepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	DepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	Result = mDevice->CreateDepthStencilState(&DepthStencilDesc, &mDepthStencilState);
	if (FAILED(Result))
	{
		return;
	}

	// Set the depth stencil state.
	mDeviceContext->OMSetDepthStencilState(mDepthStencilState, 1);

	// Initailze the depth stencil view.
	ZeroMemory(&DepthStencilViewDesc, sizeof(DepthStencilViewDesc));

	// Set up the depth stencil view description.
	DepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DepthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	Result = mDevice->CreateDepthStencilView(mDepthStencilBuffer, &DepthStencilViewDesc, &mDepthStencilView);
	if (FAILED(Result))
	{
		return;
	}

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

	// Setup the viewport for rendering.
	Viewport.Width = static_cast<float>(Width);
	Viewport.Height = static_cast<float>(Height);
	Viewport.MinDepth = 0.0f;
	Viewport.MaxDepth = 1.0f;
	Viewport.TopLeftX = 0.0f;
	Viewport.TopLeftY = 0.0f;

	// Create the viewport.
	mDeviceContext->RSSetViewports(1, &Viewport);
}

void RendererD3D11::Draw()
{

}

void RendererD3D11::Shutdown()
{
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	if (mSwapChain)
	{
		mSwapChain->SetFullscreenState(false, NULL);
	}

	if (mDepthStencilView)
	{
		mDepthStencilView->Release();
		mDepthStencilView = 0;
	}

	if (mDepthStencilState)
	{
		mDepthStencilState->Release();
		mDepthStencilState = 0;
	}

	if (mDepthStencilBuffer)
	{
		mDepthStencilBuffer->Release();
		mDepthStencilBuffer = 0;
	}

	if (mRenderTargetView)
	{
		mRenderTargetView->Release();
		mRenderTargetView = 0;
	}

	if (mDeviceContext)
	{
		mDeviceContext->Release();
		mDeviceContext = 0;
	}

	if (mDevice)
	{
		mDevice->Release();
		mDevice = 0;
	}

	if (mSwapChain)
	{
		mSwapChain->Release();
		mSwapChain = 0;
	}
}