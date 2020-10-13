#pragma once
#include <core/core.hpp>

//fix it pls
#undef CreateWindow
#include <app_framework/base/platform_window.hpp>

#include "ray_renderer_core_class_helper.hpp"
#include "descriptor_heap.hpp"
#include "resources/gpu_buffer.hpp"
#include "root_signature.hpp"
#include "pipeline_state.hpp"
#include "resources/texture_manager.hpp"

#include <vector>
#include <core\module\module_meta.hpp>
#include <d3d12.h>
#include <dxgi1_4.h>


#ifdef RAY_BUILD_RENDERER_CORE
#define RAY_RENDERERCORE_API __declspec(dllexport)
#else
#define RAY_RENDERERCORE_API RAY_DLLIMPORT
#endif

namespace ray::renderer_core_api
{
	class CommandListManager;
	class CommandContext;
	class ContextManager;

	namespace globals
	{
		extern CommandListManager gCommandListManager;
		extern ContextManager gContextManager;
		extern ID3D12Device* gDevice;
		extern DescriptorAllocator gDescriptorAllocator[];
	}

	inline D3D12_CPU_DESCRIPTOR_HANDLE RAY_RENDERERCORE_API AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE type, u32 count = 1)
	{
		return globals::gDescriptorAllocator[type].Allocate(count);
	}

	struct RAY_RENDERERCORE_API IRenderer final
	{
		IRenderer()
			: _swapChain(nullptr)
		{}

		void Initialize(ray::core::IPlatformWindow* window);
		void Shutdown();

		void BeginScene(GraphicsContext& gfxContext);
		void EndScene(GraphicsContext& gfxContext);

		static bool IsReady() { return _sbReady; }


	private:
		static bool _sbReady;
		IDXGISwapChain1* _swapChain;
		/*resources::GpuBuffer _vertexBuffer;
		resources::GpuBuffer _indexBuffer;
		RootSignature _rootSignature;
		GraphicsPipeline _pipeline;
		D3D12_VERTEX_BUFFER_VIEW _vertexBufferView;
		D3D12_INDEX_BUFFER_VIEW _indexBufferView;*/

	};

using GetRRCClassHelper_t = IRRCClassHelper* (*)();

}