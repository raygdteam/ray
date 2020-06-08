#include "command_list.hpp"
#include <d3d12.h>

namespace ray::renderer::d3d12
{
	bool D3D12CommandList::Reset(ICommandAllocator* commandAllocator, IPipelineState* pipelineState)
	{
		auto tempAllocator = static_cast<ID3D12CommandAllocator*>(commandAllocator->GetInstance());
		ID3D12PipelineState* tempState = nullptr;
		if(pipelineState)
			tempState = static_cast<ID3D12PipelineState*>(pipelineState->GetInstance());
		auto tempList = static_cast<ID3D12GraphicsCommandList*>(GetInstance());

		auto hResult = tempList->Reset(tempAllocator, tempState);

		return (hResult == S_OK) ? true : false;
	}

	void D3D12CommandList::Close()
	{
		auto temp = static_cast<ID3D12GraphicsCommandList*>(GetInstance());
		temp->Close();
	}
}