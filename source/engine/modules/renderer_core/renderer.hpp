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

#include "core/math/vector.hpp"

class CommandListManager;
class CommandContext;
class ContextManager;

class DescriptorAllocator;
class DescriptorHeap;

class GraphicsPipeline;
class RootSignature;


template<typename TGpuMemoryPool>
class GpuResourceAllocator;

class GpuTextureMemoryPool;
class GpuBufferAllocator;
class GpuPixelBufferMemoryPool;

class GpuBuffer;
class RingBuffer;
class UploadBuffer;
class ColorBuffer;

// ============================= GLOBAL VARIABLES ============================= //

extern CommandListManager gCommandListManager;
extern ContextManager gContextManager;
extern ID3D12Device* gDevice;

extern GpuResourceAllocator<GpuTextureMemoryPool> gTextureAllocator;
extern GpuResourceAllocator<GpuBufferMemoryPool> gBufferAllocator;
extern GpuResourceAllocator<GpuPixelBufferMemoryPool> gPixelBufferAllocator;

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

typedef struct RAY_RENDERERCORE_API RendererStats
{
	size_t DrawCallsCount = 0u;

	size_t AllocatedGpuMemory = 0u;
	size_t UsedGpuMemory = 0u;

	size_t AllocatedVirtualMemory = 0u;
	size_t UsedVirtualMemory = 0u;
} RendererStats_t;

typedef struct RAY_RENDERERCORE_API RendererInfo
{
	size_t MaxGpuMemorySize = 0u;

	bool bMultiEngine;
	bool bHardwareRayTracing;
	bool bReservedResources;
	bool bArgsBuffer;
	bool bHeapTier2; // dx 12
	// TODO: 

	RendererName Name;
} RendererInfo_t;

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

public:
	static RendererStats_t sRendererStats;
	static RendererInfo_t sRendererInfo;

private:
	void PreparePresentObjects() noexcept;

public:
	IRenderer()
		: _swapChain(nullptr)
		, _srvDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, 1)
	{}

	IRenderer(IRenderer&& rhs) = default;
	IRenderer& operator = (IRenderer&& rhs) = default;

public:
	void Initialize(IPlatformWindow* window) noexcept;
	void Shutdown() noexcept;

public:
	void Begin(ColorBuffer& renderTarget, GraphicsContext& gfxContext) noexcept;
	void End(ColorBuffer& renderTarget, GraphicsContext& gfxContext) noexcept;

public:
	void Present(ColorBuffer& finalFrame, GraphicsContext& gfxContext) noexcept;

public:
	static bool IsReady() noexcept { return _sbReady; }
	void OnResize(u32 width, u32 height);
};

// using GetRRCClassHelper_t = IRRCClassHelper* (*)();
