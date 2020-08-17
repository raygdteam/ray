#pragma once
#include <core/core.hpp>

//fix it pls
#undef CreateWindow
#include <app_framework/base/platform_window.hpp>

#include "ray_renderer_core_class_helper.hpp"
#include "descriptor_heap.hpp"

#include <vector>
#include <core\module\module_meta.hpp>
#include <d3d12.h>



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

	D3D12_CPU_DESCRIPTOR_HANDLE AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE type, u32 count = 1)
	{
		return globals::gDescriptorAllocator[type].Allocate(count);
	}

	struct RAY_RENDERERCORE_API IRenderer final// : public Object
	{
		void Initialize(ray::core::IPlatformWindow* window);
		void Shutdown();

		void BeginScene();
		void EndScene();
		void Execute();

		static bool IsReady() { return _sbReady; }

	private:
		void WaitForPreviousFrame(); //temporary


	private:
		static bool _sbReady;

	};

using GetRRCClassHelper_t = IRRCClassHelper* (*)();

}