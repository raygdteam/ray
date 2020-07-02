#include "pch.hpp"
#include "d3d12_swap_chain.hpp"

namespace ray::renderer::d3d12
{

	bool D3D12SwapChain::Initialize(SwapChainDesc& desc, u32& frameIndex)
	{
        HRESULT hResult;
        IDXGISwapChain3* swapChain;

        DXGI_MODE_DESC backBufferDesc = {}; // this is to describe our display mode
        backBufferDesc.Width = desc._back_buffer._width; // buffer width
        backBufferDesc.Height = desc._back_buffer._height; // buffer height
        backBufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // format of the buffer (rgba 32 bits, 8 bits for each chanel)

        // describe our multi-sampling. We are not multi-sampling, so we set the count to 1 (we need at least one sample of course)
        DXGI_SAMPLE_DESC sampleDesc = {};
        sampleDesc.Count = 1; // multisample count (no multisampling, so we just put 1, since we still need 1 sample)

        // Describe and create the swap chain.
        DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
        swapChainDesc.BufferCount = desc._buffer_count; // number of buffers we have
        swapChainDesc.BufferDesc = backBufferDesc; // our back buffer description
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // this says the pipeline will render to this swap chain
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // dxgi will discard the buffer (data) after we call present
        swapChainDesc.OutputWindow = static_cast<HWND>(desc._output_window); // handle to our window
        swapChainDesc.SampleDesc = sampleDesc; // our multi-sampling description
        swapChainDesc.Windowed = !desc._fullscreen; // set to true, then if in fullscreen must call SetFullScreenState with true for full screen to get uncapped fps

        IDXGISwapChain* tempSwapChain;

        IDXGIFactory4* dxgiFactory;
        hResult = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));

        if (FAILED(hResult))
            return false;

        hResult = dxgiFactory->CreateSwapChain(
            static_cast<IUnknown*>(desc._command_queue->GetInstance()), // the queue will be flushed once the swap chain is created
            &swapChainDesc, // give it the swap chain description we created above
            &tempSwapChain // store the created swap chain in a temp IDXGISwapChain interface
        );

        if (FAILED(hResult))
            return false;

        swapChain = static_cast<IDXGISwapChain3*>(tempSwapChain);
        frameIndex = swapChain->GetCurrentBackBufferIndex();

        SetInstance(swapChain);

        return true;
	}

    bool D3D12SwapChain::GetBuffer(u32 index, IResource* resource)
    {
        ID3D12Resource* d3d12Resource;
        auto temp = static_cast<IDXGISwapChain3*>(GetInstance());
        auto hResult = temp->GetBuffer(index, IID_PPV_ARGS(&d3d12Resource));
        if (FAILED(hResult))
            return false;

        resource->SetInstance(d3d12Resource);
        return true;
    }

    u32 D3D12SwapChain::GetCurrentBackBufferIndex()
    {
        return static_cast<IDXGISwapChain3*>(GetInstance())->GetCurrentBackBufferIndex();
    }

    bool D3D12SwapChain::Present(u32 syncInterval, u32 flags)
    {
        auto temp = static_cast<IDXGISwapChain3*>(GetInstance());
        auto hResult = temp->Present(syncInterval, flags);

        if (hResult == DXGI_ERROR_DEVICE_REMOVED || hResult == DXGI_ERROR_DEVICE_RESET)
            return false;

        return hResult == S_OK;
    }

    D3D12SwapChain::~D3D12SwapChain()
    {
        auto temp = static_cast<IDXGISwapChain3*>(GetInstance());
        BOOL fs = static_cast<BOOL>(false);

        temp->GetFullscreenState(&fs, nullptr);
        if (fs == TRUE)
            temp->SetFullscreenState(false, nullptr);

        if (temp)
            temp->Release();
    }
}
