#pragma once
#include "ray_renderer_core_class_helper.hpp"

namespace ray::renderer_core_api
{
#define GPU_VIRTUAL_ADDRESS_NULL (static_cast<GpuVirtualAddress>(0))

	using GpuVirtualAddress = u64;
	
	IDevice* gDevice = nullptr;
	IRRCClassHelper* gClassHelper = nullptr;
	const u32 NumDescriptorTypes = 4;
	DescriptorAllocator gDescriptorAllocator[NumDescriptorTypes] =
	{
		DescriptorHeapType::eRTV,
		DescriptorHeapType::eDSV,
		DescriptorHeapType::eSampler,
		DescriptorHeapType::eUAV_SRV_CBV
	};

	extern ContextManager gContextManager;

	extern CommandListManager gCommandManager;
}
