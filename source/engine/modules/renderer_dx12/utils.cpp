#include "pch.hpp"
#include "utils.hpp"

namespace ray::renderer::d3d12::utils
{
	DXGI_FORMAT ConvertShaderTypeToDXGI(ShaderType type)
	{
		switch (type)
		{
		case ray::renderer_core_api::resources::ShaderType::eUnknown:
			return DXGI_FORMAT_UNKNOWN;

		case ray::renderer_core_api::resources::ShaderType::eTypeless4:
			return DXGI_FORMAT_R32G32B32A32_TYPELESS;

		case ray::renderer_core_api::resources::ShaderType::eFloat4:
			return DXGI_FORMAT_R32G32B32A32_FLOAT;

		case ray::renderer_core_api::resources::ShaderType::eInt4:
			return DXGI_FORMAT_R32G32B32A32_SINT;

		case ray::renderer_core_api::resources::ShaderType::eUint4:
			return DXGI_FORMAT_R32G32B32A32_UINT;

		case ray::renderer_core_api::resources::ShaderType::eTypeless3:
			return DXGI_FORMAT_R32G32B32_TYPELESS;

		case ray::renderer_core_api::resources::ShaderType::eFloat3:
			return DXGI_FORMAT_R32G32B32_FLOAT;

		case ray::renderer_core_api::resources::ShaderType::eInt3:
			return DXGI_FORMAT_R32G32B32_SINT;

		case ray::renderer_core_api::resources::ShaderType::eUint3:
			return DXGI_FORMAT_R32G32B32_UINT;

		default:
			return DXGI_FORMAT_UNKNOWN;
		}
	}

	D3D12_PRIMITIVE_TOPOLOGY_TYPE ConvertPrimitiveTopologyTypeToD3D12(PrimitiveTopology topology)
	{
		switch (topology)
		{
		case ray::renderer_core_api::PrimitiveTopology::eUndefined:
			return D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED;

		case ray::renderer_core_api::PrimitiveTopology::ePoint:
			return D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

		case ray::renderer_core_api::PrimitiveTopology::eLine:
			return D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;

		case ray::renderer_core_api::PrimitiveTopology::eTriangle:
			return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		case ray::renderer_core_api::PrimitiveTopology::ePatch:
			return D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;

		default:
			return D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED;
			
		}
	}

	UINT GetShaderTypeSize(ShaderType type)
	{
		switch (type)
		{
		case ray::renderer_core_api::resources::ShaderType::eTypeless4:
			return static_cast<UINT>(0);

		case ray::renderer_core_api::resources::ShaderType::eFloat4:
			return static_cast<UINT>(sizeof(float) * 4);

		case ray::renderer_core_api::resources::ShaderType::eInt4:
			return static_cast<UINT>(sizeof(s32) * 4);

		case ray::renderer_core_api::resources::ShaderType::eUint4:
			return static_cast<UINT>(sizeof(u32) * 4);

		case ray::renderer_core_api::resources::ShaderType::eTypeless3:
			return static_cast<UINT>(0);

		case ray::renderer_core_api::resources::ShaderType::eFloat3:
			return static_cast<UINT>(sizeof(float) * 3);

		case ray::renderer_core_api::resources::ShaderType::eInt3:
			return static_cast<UINT>(sizeof(s32) * 3);

		case ray::renderer_core_api::resources::ShaderType::eUint3:
			return static_cast<UINT>(sizeof(u32) * 3);

		default:
			return static_cast<UINT>(0);

		}
	}

	D3D12_HEAP_TYPE ConvertResourceUsageToHeapType(ResourceUsage usage)
	{
		switch (usage)
		{
		case ray::renderer_core_api::resources::ResourceUsage::eDefault:
			return D3D12_HEAP_TYPE_DEFAULT;

		case ray::renderer_core_api::resources::ResourceUsage::eUpload:
			return D3D12_HEAP_TYPE_UPLOAD;

		case ray::renderer_core_api::resources::ResourceUsage::eReadback:
			return D3D12_HEAP_TYPE_READBACK;

		default:
			return D3D12_HEAP_TYPE_DEFAULT;
		}
	}
	D3D12_RESOURCE_DIMENSION ConvertResourceTypeToResourceDimension(ResourceType type)
	{
		switch (type)
		{
		case ray::renderer_core_api::resources::ResourceType::eBuffer:
			return D3D12_RESOURCE_DIMENSION_BUFFER;

		case ray::renderer_core_api::resources::ResourceType::eTexture1D:
			return D3D12_RESOURCE_DIMENSION_TEXTURE1D;

		case ray::renderer_core_api::resources::ResourceType::eTexture2D:
			return D3D12_RESOURCE_DIMENSION_TEXTURE2D;

		case ray::renderer_core_api::resources::ResourceType::eTexture3D:
			return D3D12_RESOURCE_DIMENSION_TEXTURE3D;

		default:
			return D3D12_RESOURCE_DIMENSION_UNKNOWN;

		}
	}
	D3D12_DESCRIPTOR_HEAP_TYPE ConvertDescriptorHeapTypeToD3D12(DescriptorHeapType type)
	{
		switch (type)
		{
		case ray::renderer_core_api::DescriptorHeapType::eRTV:
			return D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

		case ray::renderer_core_api::DescriptorHeapType::eUAV_SRV_CBV:
			return D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

		case ray::renderer_core_api::DescriptorHeapType::eSampler:
			return D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;

		case ray::renderer_core_api::DescriptorHeapType::eDSV:
			return D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

		default:
			return D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		}
	}
	D3D12_COMMAND_LIST_TYPE ConvertCommandListTypeToD3D12(CommandListType type)
	{
		switch (type)
		{
		case ray::renderer_core_api::CommandListType::eDirect:
			return D3D12_COMMAND_LIST_TYPE_DIRECT;

		case ray::renderer_core_api::CommandListType::eBundle:
			return D3D12_COMMAND_LIST_TYPE_BUNDLE;

		case ray::renderer_core_api::CommandListType::eCompute:
			return D3D12_COMMAND_LIST_TYPE_COMPUTE;

		case ray::renderer_core_api::CommandListType::eCopy:
			return D3D12_COMMAND_LIST_TYPE_COPY;

		default:
			return D3D12_COMMAND_LIST_TYPE_DIRECT;
		}
	}
	D3D12_RESOURCE_STATES ConvertResourceStateToD3D12(ResourceState state)
	{
		switch (state)
		{
		case ray::renderer_core_api::resources::ResourceState::ePresent:
			return D3D12_RESOURCE_STATE_PRESENT;

		case ray::renderer_core_api::resources::ResourceState::eCopyDest:
			return D3D12_RESOURCE_STATE_COPY_DEST;

		case ray::renderer_core_api::resources::ResourceState::eRenderTarget:
			return D3D12_RESOURCE_STATE_RENDER_TARGET;

		case ray::renderer_core_api::resources::ResourceState::eVertexAndConstantBuffer:
			return D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;

		case ray::renderer_core_api::resources::ResourceState::eCommon:
			return D3D12_RESOURCE_STATE_COMMON;

		case ray::renderer_core_api::resources::ResourceState::eUnorderedAccess:
			return D3D12_RESOURCE_STATE_UNORDERED_ACCESS;

		case ray::renderer_core_api::resources::ResourceState::eGenericRead:
			return D3D12_RESOURCE_STATE_GENERIC_READ;

		default:
			return D3D12_RESOURCE_STATE_COMMON;

		}
	}

	D3D12_TEXTURE_LAYOUT ConvertTextureLayoutToD3D12(TextureLayout layout)
	{
		switch (layout)
		{
		case ray::renderer_core_api::resources::TextureLayout::eUnknown:
			return D3D12_TEXTURE_LAYOUT_UNKNOWN;
		case ray::renderer_core_api::resources::TextureLayout::eRowMajor:
			return D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		default:
			return D3D12_TEXTURE_LAYOUT_UNKNOWN;
		}
	}

}