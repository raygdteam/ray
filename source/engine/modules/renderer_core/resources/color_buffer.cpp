#include "color_buffer.hpp"
#include <renderer_core/renderer.hpp>
#include <core/debug/assert.hpp>

void ColorBuffer::CreateFromSwapChain(ID3D12Resource* inResource)
{
	auto desc = inResource->GetDesc();
	_desc.Width = desc.Width;
	_desc.Height = desc.Height;
	_desc.ArraySize = desc.DepthOrArraySize;
	_desc.Format = desc.Format;
	_usageState = D3D12_RESOURCE_STATE_PRESENT;
	_resource = inResource;
	_bManaged = false;

	_desc.MipLevels = 1;
	_desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET | D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;
	_view.Create(*this);
	gDevice->CreateRenderTargetView(inResource, nullptr, GetRTV());
}

void ColorBuffer::Create(u32 width, u32 height, u32 numMips, DXGI_FORMAT format, D3D12_RESOURCE_FLAGS flags)
{
	auto desc = GpuTextureDescription::Texture2D(width, height, format, 1, flags);

	D3D12_CLEAR_VALUE clearValue;
	clearValue.Format = format;
	clearValue.Color[0] = _clearColor[0];
	clearValue.Color[1] = _clearColor[1];
	clearValue.Color[2] = _clearColor[2];
	clearValue.Color[3] = _clearColor[3];

	GpuTexture::Create(desc);
	_view.Create(*this);
}

void ColorBuffer::CreateArray(u32 width, u32 height, u32 numMips, u32 arrayCount, DXGI_FORMAT format, D3D12_RESOURCE_FLAGS flags)
{
	auto desc = GpuTextureDescription::Texture2D(width, height, format, arrayCount, flags);

	D3D12_CLEAR_VALUE clearValue;
	clearValue.Format = format;
	clearValue.Color[0] = _clearColor[0];
	clearValue.Color[1] = _clearColor[1];
	clearValue.Color[2] = _clearColor[2];
	clearValue.Color[3] = _clearColor[3];

	GpuTexture::Create(desc);
	_view.Create(*this);
}

void ColorBuffer::GenerateMipMaps(CommandContext& context)
{
	// TODO: RootSignature & ComputeContext
}
