#include "command_list.hpp"
#include "d3dx12.h"
#include <d3d12.h>

namespace ray::renderer::d3d12
{
	bool D3D12CommandList::Reset(ICommandAllocator* commandAllocator, IPipelineState* pipelineState)
	{
		auto tempAllocator = static_cast<ID3D12CommandAllocator*>(commandAllocator->GetInstance());
		ID3D12PipelineState* tempState = nullptr;
		if (pipelineState)
			tempState = static_cast<ID3D12PipelineState*>(pipelineState->GetInstance());
		auto tempList = static_cast<ID3D12GraphicsCommandList*>(GetInstance());

		auto hResult = tempList->Reset(tempAllocator, tempState);

		return (hResult == S_OK) ? true : false;
	}

	void D3D12CommandList::ResourceBarrier(resources::IResourceBarrier* inResourceBarrier, u32 numBarriers)
	{
		auto tempList = static_cast<ID3D12GraphicsCommandList*>(GetInstance());
		auto tempResourceBarrier = static_cast<CD3DX12_RESOURCE_BARRIER*>(inResourceBarrier->GetInstance());
		tempList->ResourceBarrier(numBarriers, tempResourceBarrier);
	}

	void D3D12CommandList::OMSetRenderTargetView(u32 numRTs, ICPUDescriptor* inRtHandle, ICPUDescriptor* inDsHandle, bool bRTsSingleHandleToDescriptorRange)
	{
		auto tempList = static_cast<ID3D12GraphicsCommandList*>(GetInstance());
		auto tempRtHandle = static_cast<CD3DX12_CPU_DESCRIPTOR_HANDLE*>(inRtHandle->GetInstance());
		CD3DX12_CPU_DESCRIPTOR_HANDLE* tempDsHandle = nullptr;
		if (inDsHandle)
			tempDsHandle = static_cast<CD3DX12_CPU_DESCRIPTOR_HANDLE*>(inDsHandle->GetInstance());

		tempList->OMSetRenderTargets(numRTs, tempRtHandle, bRTsSingleHandleToDescriptorRange, tempDsHandle);
	}

	void D3D12CommandList::ClearRenderTarget(ICPUDescriptor* inRtHandle, float* rgba)
	{
		auto tempList = static_cast<ID3D12GraphicsCommandList*>(GetInstance());
		auto tempHandle = static_cast<CD3DX12_CPU_DESCRIPTOR_HANDLE*>(inRtHandle->GetInstance());
		tempList->ClearRenderTargetView(*tempHandle, rgba, 0, nullptr);
	}

	bool D3D12CommandList::Close()
	{
		auto temp = static_cast<ID3D12GraphicsCommandList*>(GetInstance());
		auto hResult = temp->Close();

		return (hResult == S_OK) ? true : false;
	}

	D3D12CommandList::~D3D12CommandList()
	{
		if(GetInstance())
			static_cast<ID3D12CommandList*>(GetInstance())->Release();
	}
}