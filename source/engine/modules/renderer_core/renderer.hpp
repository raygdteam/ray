#pragma once
#include <core/core.hpp>
#include <app_framework/base/platform_window.hpp>

#include "ray_renderer_core_class_helper.hpp"

#include <vector>

#ifdef RAY_BUILD_RENDERER_CORE
#define RAY_RENDERERCORE_API __declspec(dllexport)
#else
#define RAY_RENDERERCORE_API RAY_DLLIMPORT
#endif

namespace ray::renderer_core_api
{
	struct RAY_RENDERERCORE_API IRenderer final// : public Object
	{
		friend class RendererCommands;

		void Initialize(ray::core::IPlatformWindow* window, IModule* rendererModule);
		void Shutdown();

		void BeginScene();
		void EndScene();
		void Execute();

		bool IsRunning() { return _running; }

		void Add3DCommandList(ICommandList* _3dCommandList) noexcept { _3dLists.push_back(_3dCommandList); }
		void AddCopyCommandList(ICommandList* copyCommandList) noexcept { _copyLists.push_back(copyCommandList); }
		void AddComputeCommandList(ICommandList* computeCommandList) noexcept { _computeLists.push_back(computeCommandList); }

		IDevice* GetDevice() const noexcept { return _device; }

	private:
		void WaitForPreviousFrame(); //temporary


	private:
		bool _running;
		resources::IResourceBarrier* _resourceBarrier;
		IRRCClassHelper* _classHelper;
		IDevice* _device;
		ICommandList* _rtvCommandList;
		std::vector<ICommandList*> _3dLists;
		std::vector<ICommandList*> _copyLists;
		std::vector<ICommandList*> _computeLists;
		IDescriptorHeap* _descriptorHeap;
		ISwapChain* _swapChain;
		ICommandQueue* _3dCommandQueue; 
		ICommandQueue* _copyCommandQueue; // is not used yet
		ICommandQueue* _computeCommandQueue; // is not used yet
		static const u32 FRAME_BUFFER_COUNT = 3;
		IFenceEvent* _fenceEvent;
		u32 _frameIndex;
		resources::IResource* _renderTargets[FRAME_BUFFER_COUNT];
		ICommandAllocator* _commandAllocators[FRAME_BUFFER_COUNT];
		IFence* _fences[FRAME_BUFFER_COUNT];
		u64 _fenceValues[FRAME_BUFFER_COUNT];

	};

using GetRRCClassHelper_t = IRRCClassHelper* (*)();

}