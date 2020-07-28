#include "pch.hpp"
#include "d3d12_command_queue.hpp"

namespace ray::renderer::d3d12  
{
	void D3D12CommandQueue::SetCommandLists(ICommandList** commandLists, size_t size)
	{
		if (!commandLists || size == 0)
			return;

		_size = size;
		if (_d3d12_lists)
			delete[] _d3d12_lists;

		_d3d12_lists = new ID3D12CommandList*[size];  

		for (size_t i = 0; i < size; i++)
		{
			_d3d12_lists[i] = static_cast<ID3D12CommandList*>(commandLists[i]->GetInstance());
		}
	}

	void D3D12CommandQueue::ExecuteCommandLists()
	{
		auto temp = static_cast<ID3D12CommandQueue*>(GetInstance());
		temp->ExecuteCommandLists(_size, _d3d12_lists);
	}

	bool D3D12CommandQueue::Signal(IFence* fence, u32 fenceValue)
	{
		auto tempQueue = static_cast<ID3D12CommandQueue*>(GetInstance());
		auto tempFence = static_cast<ID3D12Fence*>(fence->GetInstance());

		auto hResult = tempQueue->Signal(tempFence, static_cast<UINT64>(fenceValue));

		return hResult == S_OK;
	}

	bool D3D12CommandQueue::Wait(IFence* fence, u64 fenceValue)
	{
		auto tempQueue = static_cast<ID3D12CommandQueue*>(GetInstance());
		auto tempFence = static_cast<ID3D12Fence*>(fence->GetInstance());

		auto hResult = tempQueue->Wait(tempFence, fenceValue);

		return hResult == S_OK;
	}

	D3D12CommandQueue::~D3D12CommandQueue()
	{
		delete _d3d12_lists;

		if(GetInstance())
			static_cast<ID3D12CommandQueue*>(GetInstance())->Release();
	}
}