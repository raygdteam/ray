#pragma once

#include <core/math/vector.hpp>

#include "command_list.hpp"
#include "device.hpp"
#include "core/module/module_meta.hpp"

namespace ray::renderer_core_api
{
	class Renderer2D
	{
	public:
		~Renderer2D();

		static void Initialize(IDevice* device, IModule* rendererModule);
		static void Begin();
		static void End();

		static void DrawQuad(math::Vector<3, float> pos, math::Vector<4, float> color);

	private:
		static ICommandAllocator* _commandAllocator;
		static ICommandList* _commandList;
		static IDevice* _device;

	private:
		static void Flush();
		static void FlushAndReset();

	};

}


