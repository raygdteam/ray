#include <renderer_core/renderer.hpp>
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>

class RendererD3D11 : public ray::IRenderer
{
public: 
	RendererD3D11();
	void Initialize(ray::core::IPlatformWindow*) override;
	void Draw() override;

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
	
}

void RendererD3D11::Draw()
{

}