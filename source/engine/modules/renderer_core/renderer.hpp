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
		void Initialize(ray::core::IPlatformWindow* window, IModule* rendererModule);
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