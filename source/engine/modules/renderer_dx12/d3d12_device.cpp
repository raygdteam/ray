#include "pch.hpp"
#include "d3d12_device.hpp"

//Direct3D 12 Device implementation

using namespace ray::renderer_core_api;

namespace ray::renderer::d3d12
{
    void D3D12Device::SetName(pcstr debugName)
    {
        IRRCBase::SetName(debugName);

        wchar_t wPath[256];
        size_t result;
        mbstowcs_s(&result, wPath, debugName, 256);
        static_cast<ID3D12Device*>(GetInstance())->SetName(wPath);
    }

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

        return true;
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
        D3D12_DESCRIPTOR_HEAP_DESC dhDesc = {};
        dhDesc.NumDescriptors = desc._num_descriptors;
        dhDesc.Type = utils::ConvertDescriptorHeapTypeToD3D12(desc._type);

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
        D3D12_RENDER_TARGET_VIEW_DESC rtvDesc;
        rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
        tempDevice->CreateRenderTargetView(tempResource, nullptr, *(tempDescriptor));
        auto hResult = tempDevice->GetDeviceRemovedReason();
    }

    bool D3D12Device::CreateCommandAllocator(ICommandAllocator* commandAllocator, CommandListType listType)
    {
        D3D12_COMMAND_LIST_TYPE type = utils::ConvertCommandListTypeToD3D12(listType);
        
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
        D3D12_COMMAND_LIST_TYPE type = utils::ConvertCommandListTypeToD3D12(listType);

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
        HANDLE fenceEvent = CreateEventA(nullptr, bManualReset, bInitialState, name);
        if (!fenceEvent)
            return false;

        outFenceEvent->SetInstance(fenceEvent);
        return true;
    }

    s32 D3D12Device::GetDescriptorHandleIncrementSize(DescriptorHeapType type)
    {
        return static_cast<ID3D12Device*>(GetInstance())->GetDescriptorHandleIncrementSize(utils::ConvertDescriptorHeapTypeToD3D12(type));
    }

    bool D3D12Device::CreateRootSignature(RootSignatureDesc& desc, IRootSignature* inRootSignature)
    {
        CD3DX12_ROOT_SIGNATURE_DESC rootDesc;
        rootDesc.Init(desc._num_params, nullptr, desc._num_static_samplers, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
        ID3DBlob* signature;
        auto hResult = D3D12SerializeRootSignature(&rootDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, nullptr);
        if (FAILED(hResult))
            return false;

        ID3D12RootSignature* rootSignature;
        hResult = static_cast<ID3D12Device*>(GetInstance())->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
        return hResult == S_OK;
    }

    bool D3D12Device::CreatePipelineState(PipelineStateDesc& inDesc, IPipelineState* inPipelineState)
    {
        D3D12_INPUT_ELEMENT_DESC* inputLayout = new D3D12_INPUT_ELEMENT_DESC[inDesc._input_layout_size];
        UINT offset = 0;
        for (size_t i = 0; i < inDesc._input_layout_size; i++)
        {
            if (i == 0)
                offset = 0;
            else
                offset += utils::GetShaderTypeSize(inDesc._input_layout[i - 1]._type);

            inputLayout[i] = { inDesc._input_layout[i]._semantic_name, 0, 
                                utils::ConvertShaderTypeToDXGI(inDesc._input_layout[i]._type), 0, 
                                offset,   
                                D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

        }

        // fill out an input layout description structure
        D3D12_INPUT_LAYOUT_DESC inputLayoutDesc = {};

        // we can get the number of elements in an array by "sizeof(array) / sizeof(arrayElementType)"
        inputLayoutDesc.NumElements = sizeof(inputLayout) / sizeof(D3D12_INPUT_ELEMENT_DESC);
        inputLayoutDesc.pInputElementDescs = inputLayout;

        DXGI_SAMPLE_DESC sampleDesc = {};
        sampleDesc.Count = 1;

        D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {}; // a structure to define a pso
        psoDesc.InputLayout = inputLayoutDesc; // the structure describing our input layout
        psoDesc.pRootSignature = static_cast<ID3D12RootSignature*>(inDesc._root_signature->GetInstance()); // the root signature that describes the input data this pso needs
        psoDesc.VS = *(static_cast<D3D12_SHADER_BYTECODE*>(inDesc._vs->GetBytecode())); // structure describing where to find the vertex shader bytecode and how large it is
        psoDesc.PS = *(static_cast<D3D12_SHADER_BYTECODE*>(inDesc._ps->GetBytecode())); // same as VS but for pixel shader
        psoDesc.PrimitiveTopologyType = utils::ConvertPrimitiveTopologyTypeToD3D12(inDesc._topology); // type of topology we are drawing
        psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // format of the render target
        psoDesc.SampleDesc = sampleDesc; // must be the same sample description as the swapchain and depth/stencil buffer
        psoDesc.SampleMask = 0xffffffff; // sample mask has to do with multi-sampling. 0xffffffff means point sampling is done
        psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT); // a default rasterizer state.
        psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT); // a default blent state.
        psoDesc.NumRenderTargets = inDesc._num_render_targets; // we are only binding one render target

        ID3D12PipelineState* pipelineStateObject;

        // create the pso
        auto hResult = static_cast<ID3D12Device*>(GetInstance())->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipelineStateObject));
        if (FAILED(hResult))
        {
            return false;
        }

        inPipelineState->SetInstance(pipelineStateObject);

        return true;
    }

    bool D3D12Device::CreateCommittedResource(ResourceDesc& inDesc, ResourceUsage usage, IResource* outResource)
    {
        auto tempDevice = static_cast<ID3D12Device*>(GetInstance());
        auto tempResource = static_cast<ID3D12Resource*>(outResource->GetInstance());
        CD3DX12_RESOURCE_DESC desc;

        switch (inDesc._resource_type)
        {
        case ray::renderer_core_api::resources::ResourceType::eBuffer:
            desc = CD3DX12_RESOURCE_DESC::Buffer(inDesc._width);
            break;
        case ray::renderer_core_api::resources::ResourceType::eTexture1D:
            desc = CD3DX12_RESOURCE_DESC::Tex1D(utils::ConvertShaderTypeToDXGI(inDesc._shader_type), inDesc._width);
            break;
        case ray::renderer_core_api::resources::ResourceType::eTexture2D:
            desc = CD3DX12_RESOURCE_DESC::Tex2D(utils::ConvertShaderTypeToDXGI(inDesc._shader_type), inDesc._width, inDesc._height);
            break;
        case ray::renderer_core_api::resources::ResourceType::eTexture3D:
            break;
        default:
            break;
        }

        D3D12_RESOURCE_STATES state;
        if (usage == ResourceUsage::eDefault)
            state = D3D12_RESOURCE_STATE_COPY_DEST;
        else if (usage == ResourceUsage::eUpload)
            state = D3D12_RESOURCE_STATE_GENERIC_READ;

        auto hResult = tempDevice->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(utils::ConvertResourceUsageToHeapType(usage)), // a default heap
            D3D12_HEAP_FLAG_NONE, // no flags
            &desc, 
            state, 
            nullptr, // optimized clear value must be null for this type of resource. used for render targets and depth/stencil buffers
            IID_PPV_ARGS(&tempResource));

        SetInstance(tempResource);

        return hResult == S_OK;
    }

    D3D12Device::~D3D12Device()
    {
        if (GetInstance())
            static_cast<ID3D12Device*>(GetInstance())->Release();
    }

}
