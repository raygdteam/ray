#include "fence.hpp"
#include <d3d12.h>

namespace ray::renderer::d3d12
{
	bool D3D12Fence::SetEventOnCompletion(IFenceEvent* fenceEvent, u64 value)
	{
		auto tempFence = static_cast<ID3D12Fence*>(GetInstance());
		auto tempEvent = static_cast<HANDLE>(fenceEvent->GetInstance());

		auto hResult = tempFence->SetEventOnCompletion(value, tempEvent);

		return (hResult == S_OK) ? true : false;
	}

	u64 D3D12Fence::GetCompletedValue()
	{
		return static_cast<ID3D12Fence*>(GetInstance())->GetCompletedValue();
	}

	u32 D3D12FenceEvent::WaitFor()
	{
		WaitFor(INFINITE);
	}

	u32 D3D12FenceEvent::WaitFor(u32 ms)
	{
		auto temp = static_cast<HANDLE>(GetInstance());
		WaitForSingleObject(temp, ms);
	}

	D3D12Fence::~D3D12Fence()
	{
		if (GetInstance())
			static_cast<ID3D12Fence*>(GetInstance())->Release();
	}

	D3D12FenceEvent::~D3D12FenceEvent()
	{
		
		if (GetInstance())
			CloseHandle(static_cast<HANDLE>(GetInstance()));
	}
}