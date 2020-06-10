#include "device.hpp"
#include "d3dx12.h"

//Direct3D 12 Device implementation

using namespace ray::renderer_core_api;

namespace ray::renderer::d3d12
{

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
            return false;

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

    bool D3D12Device::CreateDescriptorHeap(DescriptorHeapDesc& desc, IDescriptorHeap* descriptorHeap)
    {
        HRESULT hResult;

        ID3D12DescriptorHeap* d3d12DescriptorHeap;
        D3D12_DESCRIPTOR_HEAP_DESC dhDesc;
        dhDesc.NumDescriptors = desc._num_descriptors;

        switch (desc._type)
        {
        case DescriptorHeapType::descriptor_heap_type_rtv:
            dhDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
            break;

        case DescriptorHeapType::descriptor_heap_type_dsv:
            dhDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
            break;

        case DescriptorHeapType::descriptor_heap_type_sampler:
            dhDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
            break;

        case DescriptorHeapType::descriptor_heap_type_uav_srv_cbv:
            dhDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
            break;

        default:
            return false;
            break;
        }

        if (desc._shader_visible)
            dhDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        else
            dhDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

        hResult = static_cast<ID3D12Device*>(GetInstance())->CreateDescriptorHeap(&dhDesc, IID_PPV_ARGS(&d3d12DescriptorHeap));
        if (FAILED(hResult))
            return false;

        descriptorHeap->SetInstance(d3d12DescriptorHeap);
        return true;

    }

    void D3D12Device::CreateRenderTargetView(IResource* resource, RenderTargetViewDesc& desc, ICPUDescriptor* descriptor)
    {
        auto tempDevice = static_cast<ID3D12Device*>(GetInstance());
        auto tempResource = static_cast<ID3D12Resource*>(resource->GetInstance());
        auto tempDescriptor = static_cast<CD3DX12_CPU_DESCRIPTOR_HANDLE*>(descriptor->GetInstance());
        tempDevice->CreateRenderTargetView(tempResource, nullptr, *tempDescriptor);
    }

    bool D3D12Device::CreateCommandAllocator(ICommandAllocator* commandAllocator, CommandListType listType)
    {
        D3D12_COMMAND_LIST_TYPE type;
        switch (listType)
        {
        case ray::renderer_core_api::CommandListType::direct:
            type = D3D12_COMMAND_LIST_TYPE_DIRECT;
            break;
        case ray::renderer_core_api::CommandListType::bundle:
            type = D3D12_COMMAND_LIST_TYPE_BUNDLE;
            break;
        case ray::renderer_core_api::CommandListType::compute:
            type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
            break;
        case ray::renderer_core_api::CommandListType::copy:
            type = D3D12_COMMAND_LIST_TYPE_COPY;
            break;
        default:
            return false;
            break;
        }

        ID3D12CommandAllocator* allocator;
        auto temp = static_cast<ID3D12Device*>(GetInstance());
        auto hResult = temp->CreateCommandAllocator(type, IID_PPV_ARGS(&allocator));
        if (FAILED(hResult))
            return false;

        commandAllocator->SetInstance(allocator);
        return true;
    }

    bool D3D12Device::CreateCommandList(ICommandList* commandList, ICommandAllocator* commandAllocator, IPipelineState* pipelineState, CommandListType listType)
    {
        D3D12_COMMAND_LIST_TYPE type;
        switch (listType)
        {
        case ray::renderer_core_api::CommandListType::direct:
            type = D3D12_COMMAND_LIST_TYPE_DIRECT;
            break;
        case ray::renderer_core_api::CommandListType::bundle:
            type = D3D12_COMMAND_LIST_TYPE_BUNDLE;
            break;
        case ray::renderer_core_api::CommandListType::compute:
            type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
            break;
        case ray::renderer_core_api::CommandListType::copy:
            type = D3D12_COMMAND_LIST_TYPE_COPY;
            break;
        default:
            return false;
            break;
        }

        auto tempDevice = static_cast<ID3D12Device*>(GetInstance());
        auto tempAllocator = static_cast<ID3D12CommandAllocator*>(commandAllocator->GetInstance());

        ID3D12PipelineState* tempState = nullptr;

        if (pipelineState)
            tempState = static_cast<ID3D12PipelineState*>(pipelineState->GetInstance());

        ID3D12CommandList* list;
        auto hResult = tempDevice->CreateCommandList(0, type, tempAllocator, tempState, IID_PPV_ARGS(&list));
        if (FAILED(hResult))
            return false;
        commandList->SetInstance(list);
        return true;
    }

    bool D3D12Device::CreateFence(IFence* outFence, u64 fenceValue)
    {
        ID3D12Fence* fence;
        auto temp = static_cast<ID3D12Device*>(GetInstance());
        auto hResult = temp->CreateFence(fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
        if (FAILED(hResult))
            return false;
        outFence->SetInstance(fence);

        return true;
    }

    bool D3D12Device::CreateFenceEvent(IFenceEvent* outFenceEvent, pcstr name, bool bManualReset, bool bInitialState)
    {
        HANDLE fenceEvent = CreateEvent(nullptr, bManualReset, bInitialState, name);
        if (!fenceEvent)
            return false;

        outFenceEvent->SetInstance(fenceEvent);
        return true;
    }

    s32 D3D12Device::GetDescriptorHandleIncrementSize(DescriptorHeapType type)
    {
        switch (type)
        {
        case DescriptorHeapType::descriptor_heap_type_rtv:
            static_cast<ID3D12Device*>(GetInstance())->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
            break;

        case DescriptorHeapType::descriptor_heap_type_dsv:
            static_cast<ID3D12Device*>(GetInstance())->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
            break;

        case DescriptorHeapType::descriptor_heap_type_sampler:
            static_cast<ID3D12Device*>(GetInstance())->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
            break;

        case DescriptorHeapType::descriptor_heap_type_uav_srv_cbv:
            static_cast<ID3D12Device*>(GetInstance())->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
            break;

        default:
            return 0;
            break;
        }
    }

    D3D12Device::~D3D12Device()
    {
        if (GetInstance())
            static_cast<ID3D12Device*>(GetInstance())->Release();
    }

}
