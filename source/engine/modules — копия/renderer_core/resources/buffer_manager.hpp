#pragma once
#include "color_buffer.hpp"
#include <renderer_core/resources/depth_buffer.hpp>

namespace resources = ray::renderer_core_api::resources;

namespace ray::renderer_core_api::globals
{
	extern const u32 SWAP_CHAIN_BUFFER_COUNT;
	extern u32 gCurrentBuffer;
	extern resources::ColorBuffer gDisplayPlane[];
	extern resources::ColorBuffer gSceneColorBuffer; // is not being used now
	extern resources::DepthBuffer gDepthBuffer;
}