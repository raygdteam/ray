#include <renderer_core/device.hpp>

#include <d3d12.h>
#include <dxgi.h>
#include <dxgi1_4.h>
#include <combaseapi.h>

//Direct3D 12 Device implementation

using namespace ray::renderer_core_api;

namespace ray::renderer::d3d12
{
    class D3D12Device : public IDevice
    {
    public:
        D3D12Device() {}

        bool Initialize() override;
        bool CreateCommandQueue(CommandQueueDesc& desc, ICommandQueue* cmdQueue) override;

    private:


    };

    bool D3D12Device::Initialize()
    {
        HRESULT hResult;

        IDXGIFactory4* dxgiFactory;
        hResult = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));

        if (FAILED(hResult))
            return false;

        IDXGIAdapter1* adapter; // adapters are the graphics card (this includes the embedded graphics on the motherboard)

        int adapterIndex = 0; // we'll start looking for directx 12  compatible graphics devices starting at index 0

        bool adapterFound = false; // set this to true when a good one was found

        // find first hardware gpu that supports d3d 12
        while (dxgiFactory->EnumAdapters1(adapterIndex, &adapter) != DXGI_ERROR_NOT_FOUND)
        {
            DXGI_ADAPTER_DESC1 desc;
            adapter->GetDesc1(&desc);

            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
            {
                // we dont want a software device
                adapterIndex++; // add this line here. Its not currently in the downloadable project
                continue;
            }

            // we want a device that is compatible with direct3d 12 (feature level 11 or higher)
            hResult = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr);
            if (SUCCEEDED(hResult))
            {
                adapterFound = true;
                break;
            }

            adapterIndex++;
        }

        if (!adapterFound)
            return false;

        ID3D12Device* device;
        hResult = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), reinterpret_cast<void**>(&device));
        if (FAILED(hResult))
            return;

        SetInstance(device);
    }

    bool D3D12Device::CreateCommandQueue(CommandQueueDesc& desc, ICommandQueue* cmdQueue)
    {
        HRESULT hResult;
        D3D12_COMMAND_QUEUE_DESC d3d12Desc = {};
        ID3D12CommandQueue* d3d12Queue;
        hResult = static_cast<ID3D12Device*>(GetInstance())->CreateCommandQueue(&d3d12Desc, IID_PPV_ARGS(&d3d12Queue));
        if (FAILED(hResult))
            return false;

        cmdQueue->SetInstance(d3d12Queue);

        return true;
    }

}

IDevice* GetRendererDevice()
{
    return new ray::renderer::d3d12::D3D12Device;
}