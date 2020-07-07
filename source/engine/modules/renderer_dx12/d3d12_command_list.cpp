#include "pch.hpp"
#include "d3d12_command_list.hpp"

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

		return hResult == S_OK;
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

	u64 D3D12CommandList::UpdateSubresource(IResource* dest, IResource* src, u64 offset, u64 indexSubresource, u64 subresourceCount)
	{
		auto tempList = static_cast<ID3D12GraphicsCommandList*>(GetInstance());
		auto tempDest = static_cast<ID3D12Resource*>(dest->GetInstance());
		auto tempSrc = static_cast<ID3D12Resource*>(src->GetInstance());

		D3D12_SUBRESOURCE_DATA data;
		data.pData = src->GetData();
		data.RowPitch = src->GetSize();
		data.SlicePitch = src->GetSize();
		
		return static_cast<u64>(UpdateSubresources(tempList, tempDest, tempSrc, offset, indexSubresource, subresourceCount, &data));
	}

	bool D3D12CommandList::Close()
	{
		auto temp = static_cast<ID3D12GraphicsCommandList*>(GetInstance());
		auto hResult = temp->Close();
		
		return hResult == S_OK;
	}

	D3D12CommandList::~D3D12CommandList()
	{
		auto d = GetInstance();
		if(GetInstance())
			static_cast<ID3D12CommandList*>(GetInstance())->Release();
	}
}