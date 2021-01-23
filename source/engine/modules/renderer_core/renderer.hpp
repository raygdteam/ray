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
class DescriptorHeap;

class GpuTextureAllocator;
class GpuBufferAllocator;
class GpuPixelBufferAllocator;

class RingBuffer;
class UploadBuffer;

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
	IRenderer()
		: _swapChain(nullptr)
	{}

	void Initialize(IPlatformWindow* window);
	void Shutdown();

	void BeginScene(GraphicsContext& gfxContext);
	void EndScene(GraphicsContext& gfxContext);

	static bool IsReady() { return _sbReady; }


private:
	static bool _sbReady;
	IDXGISwapChain1* _swapChain;

};

// using GetRRCClassHelper_t = IRRCClassHelper* (*)();
