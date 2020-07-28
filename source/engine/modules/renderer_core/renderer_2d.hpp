#pragma once
#include <core/math/vector2.hpp>
#include <core/math/vector3.hpp>
#include <core/math/vector4.hpp>
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

		static void DrawQuad(math::Vector3 pos, math::Vector4 color);

	private:
		static ICommandAllocator* _commandAllocator;
		static ICommandList* _commandList;
		static IDevice* _device;

	private:
		static void Flush();
		static void FlushAndReset();

	};

}


