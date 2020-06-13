#include "pch.hpp"
#include "d3d12_descriptor_heap.hpp"

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
		u32 size = GetDescriptorSize();
		auto temp = static_cast<CD3DX12_CPU_DESCRIPTOR_HANDLE*>(GetInstance());
		temp->Offset(step, GetDescriptorSize());
	
		return true;
	}

	D3D12DescriptorHeap::~D3D12DescriptorHeap()
	{
		if (GetInstance())
			static_cast<ID3D12DescriptorHeap*>(GetInstance())->Release();
	}
}