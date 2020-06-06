#include "descriptor_heap.hpp"
#include "d3dx12.h"

namespace ray::renderer::d3d12
{
	bool D3D12CPUDescriptor::Initialize(IDescriptorHeap* descriptorHeap)
	{
		auto temp = static_cast<ID3D12DescriptorHeap*>(descriptorHeap->GetInstance());
		CD3DX12_CPU_DESCRIPTOR_HANDLE descriptorHandle(temp->GetCPUDescriptorHandleForHeapStart());
		SetInstance(&descriptorHandle);

		return true; 
	}

	bool D3D12CPUDescriptor::Offset(u32 step)
	{
		if (GetDescriptorSize() == 0)
			return false;
		auto temp = static_cast<CD3DX12_CPU_DESCRIPTOR_HANDLE*>(GetInstance());
		temp->Offset(1, GetDescriptorSize());
	
		return true;
	}
}