#pragma once

namespace ray::renderer::d3d12::utils
{
	DXGI_FORMAT ConvertShaderTypeToDXGI(ShaderType type);
	D3D12_PRIMITIVE_TOPOLOGY_TYPE ConvertPrimitiveTopologyTypeToD3D12(PrimitiveTopology topology);
	UINT GetShaderTypeSize(ShaderType type);
	D3D12_HEAP_TYPE ConvertResourceUsageToHeapType(ResourceUsage usage);
	D3D12_RESOURCE_DIMENSION ConvertResourceTypeToResourceDimension(ResourceType type);
	D3D12_DESCRIPTOR_HEAP_TYPE ConvertDescriptorHeapTypeToD3D12(DescriptorHeapType type);
	D3D12_COMMAND_LIST_TYPE ConvertCommandListTypeToD3D12(CommandListType type);
}
