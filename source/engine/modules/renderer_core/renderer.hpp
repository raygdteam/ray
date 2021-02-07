#pragma once

//fix it pls
#undef CreateWindow
#include <app_framework/base/platform_window.hpp>

#include "ray_renderer_core_class_helper.hpp"
#include "resources/gpu_buffer.hpp"
#include "root_signature.hpp"
#include "pipeline_state.hpp"
#include "renderer_core.hpp"

#include <vector>
#include <core\module\module_meta.hpp>
#include <d3d12.h>
#include <dxgi1_4.h>

class CommandListManager;
class CommandContext;
class ContextManager;

class DescriptorHeapsManager;
class DescriptorAllocator;
class DescriptorHeap;

class GraphicsPipeline;
class RootSignature;

class GpuTextureAllocator;
class GpuBufferAllocator;
class GpuPixelBufferAllocator;

class GpuBuffer;
class RingBuffer;
class UploadBuffer;
class ColorBuffer;

// ============================= GLOBAL VARIABLES ============================= //

extern CommandListManager gCommandListManager;
extern ContextManager gContextManager;
extern ID3D12Device* gDevice;

extern GpuTextureAllocator gTextureAllocator;
extern GpuBufferAllocator gBufferAllocator;
extern GpuPixelBufferAllocator gPixelBufferAllocator;

extern DescriptorHeap gMainDescriptorHeap;
extern DescriptorHeapsManager gDescriptorHeapsManager;
extern RingBuffer gRingBuffer;
extern RAY_RENDERERCORE_API UploadBuffer* gUploadBuffer;
extern DescriptorAllocator gDescriptorAllocator[];

inline D3D12_CPU_DESCRIPTOR_HANDLE RAY_RENDERERCORE_API AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE type, u32 count = 1)
{
	return gDescriptorAllocator[type].Allocate(count);
}

enum RendererName : u8
{
	eDirectX11, // ???
	eOpenGL, // ???
	eVulkan,
	eDirectX12
};

struct RAY_RENDERERCORE_API RendererInfo
{
	bool bMultiEngine;
	bool bRayTracing;
	bool bReservedResources;
	bool bArgsBuffer;
	// TODO: 

	RendererName Name;
};

struct RAY_RENDERERCORE_API IRenderer final
{
private:
	static bool _sbReady;
	IDXGISwapChain1* _swapChain;
	GraphicsPipeline _presentPipeline;
	RootSignature _presentSignature;
	DescriptorHeap _srvDescriptorHeap;
	GpuBuffer _presentVB;
	BufferView _vbView;
	GpuBuffer _presentIB;
	BufferView _ibView;

private:
	void PreparePresentObjects() noexcept;

public:
	IRenderer()
		: _swapChain(nullptr)
		, _srvDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, 1)
	{}

	void Initialize(IPlatformWindow* window) noexcept;
	void Shutdown() noexcept;

	void Begin(ColorBuffer& renderTarget, GraphicsContext& gfxContext) noexcept;
	void End(ColorBuffer& renderTarget, GraphicsContext& gfxContext) noexcept;

	void Present(ColorBuffer& finalFrame, GraphicsContext& gfxContext) noexcept;

	static bool IsReady() { return _sbReady; }

};

// using GetRRCClassHelper_t = IRRCClassHelper* (*)();
