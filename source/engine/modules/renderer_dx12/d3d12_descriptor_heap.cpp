#include "pch.hpp"
#include "d3d12_descriptor_heap.hpp"

namespace ray::renderer::d3d12
{

	template<typename DescriptorHandleType>
	DescriptorHandleType* GetDescriptorHandleForHeapStart(D3D12DescriptorHeap* heap, bool bShaderVisible)
	{
		auto temp = static_cast<ID3D12DescriptorHeap*>(heap->GetInstance());
		DescriptorHandleType* src;
		if (bShaderVisible)
			src = temp->GetGPUDescriptorHandleForHeapStart();
		else
			src = temp->GetCPUDescriptorHandleForHeapStart();
		
		DescriptorHandleType* dest = new DescriptorHandleType;
		memcpy(dest, &src, sizeof(DescriptorHandleType));
		return dest;
	}

	void D3D12DescriptorHeap::GetCPUDescriptorHandleForHeapStart(ICPUDescriptor* outHandle)
	{
		outHandle->SetInstance(GetDescriptorHandleForHeapStart<D3D12_CPU_DESCRIPTOR_HANDLE>(this, false));
	}

	void D3D12DescriptorHeap::GetGPUDescriptorHandleForHeapStart(IGPUDescriptor* outHandle)
	{
		outHandle->SetInstance(GetDescriptorHandleForHeapStart<D3D12_GPU_DESCRIPTOR_HANDLE>(this, true));
	}

	bool D3D12CPUDescriptor::Offset(u32 step)
	{
		if (GetDescriptorSize() == 0)
			return false;
		auto temp = static_cast<D3D12_CPU_DESCRIPTOR_HANDLE*>(GetInstance());
		temp->ptr += step * GetDescriptorSize();
	
		return true;
	}

	bool D3D12CPUDescriptor::Increment()
	{
		return Offset(1);
	}

	D3D12CPUDescriptor::~D3D12CPUDescriptor()
	{
		if (GetInstance())
			delete GetInstance();
	}

	D3D12DescriptorHeap::~D3D12DescriptorHeap()
	{
		if (GetInstance())
			static_cast<ID3D12DescriptorHeap*>(GetInstance())->Release();
	}
}